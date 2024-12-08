#include <M5CoreS3.h>
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "BootManager.h"
#include "HUSKYLENS.h"

// Instances globales
DisplayManager display;
ConfigManager config;
BootManager bootManager(display);
HUSKYLENS huskylens;

// Variables d'état
SystemState currentState = SystemState::BOOTING;
bool qrCodeVisible = false;
unsigned long lastUpdate = 0;

// Prototypes de fonctions
void handleButtons();
void handleSystemState();
void setupWiFi();
void showQRCode();
void hideQRCode();
void enterSettingsMode();
void exitSettingsMode();

void setup() {
    // Initialisation de base
    M5.begin();
    Serial.begin(115200);
    
    // Initialiser la configuration
    config.begin();
    
    // Initialiser l'affichage
    display.begin();
    display.setBrightness(config.getBrightness());
    
    // Démarrer la séquence de démarrage
    bootManager.begin();
    
    // Si premier démarrage, préparer le QR code
    if (config.systemConfig.firstBoot) {
        String ssid = "M5HuskyLens_" + String((uint32_t)ESP.getEfuseMac(), HEX);
        String password = "12345678"; // Mot de passe par défaut
        display.generateWifiQR(ssid.c_str(), password.c_str());
        config.setWiFiCredentials(ssid.c_str(), password.c_str());
    }
}

void loop() {
    M5.update(); // Mise à jour des boutons
    
    // Gestion des boutons
    handleButtons();
    
    // Mise à jour selon l'état du système
    handleSystemState();
    
    // Mise à jour de l'affichage
    display.update();
    
    // Délai pour éviter une utilisation CPU excessive
    delay(10);
}

void handleButtons() {
    if (M5.BtnA.wasPressed()) {
        // Bouton A : Toggle QR Code
        if (currentState == SystemState::READY) {
            if (qrCodeVisible) {
                hideQRCode();
            } else {
                showQRCode();
            }
        }
    }
    
    if (M5.BtnB.wasPressed()) {
        // Bouton B : Paramètres
        if (currentState == SystemState::READY) {
            enterSettingsMode();
        } else if (currentState == SystemState::SETTINGS) {
            exitSettingsMode();
        }
    }
    
    if (M5.BtnC.wasPressed()) {
        // Bouton C : Action contextuelle
        switch (currentState) {
            case SystemState::SETTINGS:
                config.saveCurrentState();
                display.showError("Paramètres sauvegardés", true);
                delay(1000);
                exitSettingsMode();
                break;
                
            default:
                break;
        }
    }
}

void handleSystemState() {
    switch (currentState) {
        case SystemState::BOOTING:
            bootManager.update();
            if (bootManager.isBootComplete()) {
                if (config.systemConfig.firstBoot) {
                    currentState = SystemState::SETUP_REQUIRED;
                    showQRCode();
                } else {
                    currentState = SystemState::READY;
                }
            }
            break;
            
        case SystemState::SETUP_REQUIRED:
            // Attendre la configuration WiFi
            if (config.isWiFiConfigured()) {
                currentState = SystemState::CONNECTING_WIFI;
                setupWiFi();
            }
            break;
            
        case SystemState::CONNECTING_WIFI:
            // Géré par les callbacks WiFi
            break;
            
        case SystemState::CONNECTING_HUSKYLENS:
            if (huskylens.begin(Wire)) {
                currentState = SystemState::READY;
                display.showBootSequence(BootStage::COMPLETE, "HuskyLens connecté");
            } else {
                display.showError("Erreur HuskyLens", true);
            }
            break;
            
        case SystemState::READY:
            // Fonctionnement normal
            if (millis() - lastUpdate > 100) { // Mise à jour toutes les 100ms
                // Mise à jour HuskyLens
                if (huskylens.request()) {
                    // Traitement des données HuskyLens
                }
                lastUpdate = millis();
            }
            break;
            
        case SystemState::ERROR:
            // Attendre une action utilisateur
            break;
            
        case SystemState::SETTINGS:
            // Interface paramètres déjà gérée par DisplayManager
            break;
    }
}

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.getWiFiSSID().c_str(), config.getWiFiPassword().c_str());
    
    display.showBootSequence(BootStage::WIFI_SETUP, "Connexion WiFi...");
    
    // La connexion sera gérée par les callbacks WiFi
    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        switch(event) {
            case SYSTEM_EVENT_STA_GOT_IP:
                currentState = SystemState::CONNECTING_HUSKYLENS;
                display.showBootSequence(BootStage::WIFI_SETUP, "WiFi connecté");
                break;
                
            case SYSTEM_EVENT_STA_DISCONNECTED:
                display.showError("Erreur WiFi", true);
                break;
                
            default:
                break;
        }
    });
}

void showQRCode() {
    qrCodeVisible = true;
    display.showQRCode(true);
}

void hideQRCode() {
    qrCodeVisible = false;
    display.showQRCode(false);
}

void enterSettingsMode() {
    currentState = SystemState::SETTINGS;
    display.setMode(DisplayMode::SETTINGS);
}

void exitSettingsMode() {
    currentState = SystemState::READY;
    display.setMode(DisplayMode::NORMAL_OPERATION);
}