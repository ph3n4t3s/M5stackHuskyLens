#include <M5CoreS3.h>
#include "Config.h"
#include "HuskyLensPlus.h"
#include "DisplayManager.h"
#include "DataProcessor.h"

// Instances globales
HuskyLensPlus huskyLens;
DisplayManager display;
DataProcessor processor;
Configuration config;

// Variables de contrôle
bool inMenu = false;
int menuIndex = 0;
unsigned long lastButtonCheck = 0;
const unsigned long BUTTON_DELAY = 200;

void handleMenu() {
    std::vector<String> menuItems = {
        "Sensibilité: " + String(config.sensitivity),
        "Zoom: " + String(config.zoomLevel),
        "Auto Learn: " + String(config.autoLearn ? "ON" : "OFF"),
        "Audio: " + String(config.audioFeedback ? "ON" : "OFF"),
        "Mode Nuit: " + String(config.nightMode ? "ON" : "OFF"),
        "Interval: " + String(config.recordingInterval)
    };
    
    display.showMenu(menuItems, menuIndex);
    
    if (millis() - lastButtonCheck < BUTTON_DELAY) return;
    
    if (M5.BtnA.wasPressed()) {
        menuIndex = (menuIndex + 1) % Constants::MENU_ITEMS;
        lastButtonCheck = millis();
    }
    
    if (M5.BtnB.wasPressed()) {
        switch (menuIndex) {
            case 0: // Sensibilité
                config.sensitivity = (config.sensitivity % 5) + 1;
                break;
            case 1: // Zoom
                config.zoomLevel = (config.zoomLevel % 5) + 1;
                break;
            case 2: // Auto Learn
                config.autoLearn = !config.autoLearn;
                break;
            case 3: // Audio
                config.audioFeedback = !config.audioFeedback;
                break;
            case 4: // Mode Nuit
                config.nightMode = !config.nightMode;
                break;
            case 5: // Interval
                config.recordingInterval = (config.recordingInterval + 500) % 5001;
                if (config.recordingInterval < 100) config.recordingInterval = 100;
                break;
        }
        lastButtonCheck = millis();
    }
    
    if (M5.BtnC.wasPressed()) {
        inMenu = false;
        lastButtonCheck = millis();
    }
}

void handleNormalOperation() {
    static HuskyMode currentMode = HuskyMode::FACE_RECOGNITION;
    static DisplayMode currentDisplay = DisplayMode::GRAPHIC_INTERFACE;
    
    if (M5.BtnA.wasPressed() && millis() - lastButtonCheck >= BUTTON_DELAY) {
        currentMode = static_cast<HuskyMode>((static_cast<int>(currentMode) + 1) % 12);
        huskyLens.setMode(currentMode);
        processor.setMode(currentMode);
        lastButtonCheck = millis();
    }
    
    if (M5.BtnB.wasPressed() && millis() - lastButtonCheck >= BUTTON_DELAY) {
        currentDisplay = static_cast<DisplayMode>((static_cast<int>(currentDisplay) + 1) % 6);
        display.setMode(currentDisplay);
        lastButtonCheck = millis();
    }
    
    if (M5.BtnC.wasPressed() && millis() - lastButtonCheck >= BUTTON_DELAY) {
        if (config.autoLearn) {
            huskyLens.learn(1);
        }
        lastButtonCheck = millis();
    }
    
    // Entrée dans le menu
    if (M5.BtnA.wasPressed() && M5.BtnC.isPressed()) {
        inMenu = true;
        menuIndex = 0;
        lastButtonCheck = millis();
    }
}

void setup() {
    M5.begin();
    
    // Initialisation des composants
    if (!huskyLens.begin()) {
        display.showError("Erreur HuskyLens!");
        delay(2000);
    }
    
    display.begin();
    processor.begin();
    
    // Message de démarrage
    display.showError("Système prêt!", true);
    delay(1000);
}

void loop() {
    M5.update();
    
    if (inMenu) {
        handleMenu();
    } else {
        handleNormalOperation();
        
        // Mise à jour des données
        huskyLens.update();
        if (huskyLens.isConnected()) {
            SensorData data = huskyLens.getData();
            processor.process(data);
            display.update(processor.getDisplayData());
        } else {
            display.showError("Connexion perdue!");
        }
    }
    
    delay(20);
}
