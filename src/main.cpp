#include <M5CoreS3.h>
#include "Config.h"
#include "HuskyLensPlus.h"
#include "DisplayManager.h"
#include "DataProcessor.h"
#include "DataLogger.h"
#include "ConfigManager.h"
#include "WiFiManager.h"
#include "GestureAnalyzer.h"

// Instances globales
HuskyLensPlus huskyLens;
DisplayManager display;
DataProcessor processor;
DataLogger logger;
ConfigManager configManager;
WiFiManager wifiManager;
GestureAnalyzer gestureAnalyzer;
Configuration config;

// Variables de contrôle
bool inMenu = false;
int menuIndex = 0;
unsigned long lastButtonCheck = 0;
unsigned long lastDataLog = 0;
unsigned long lastWiFiCheck = 0;
const unsigned long BUTTON_DELAY = 200;
const unsigned long DATA_LOG_INTERVAL = 1000;
const unsigned long WIFI_CHECK_INTERVAL = 5000;

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

void onConfigChange(const Configuration& newConfig) {
    config = newConfig;
    display.setMode(config.nightMode ? DisplayMode::DEBUGGING_VIEW : DisplayMode::GRAPHIC_INTERFACE);
}

void handleGestures(const SensorData& data) {
    if (data.points.empty()) return;
    
    for (const auto& point : data.points) {
        gestureAnalyzer.addPoint(point);
    }
    
    if (gestureAnalyzer.isGestureInProgress()) {
        String gesture = gestureAnalyzer.recognizeGesture();
        if (!gesture.isEmpty()) {
            float confidence = gestureAnalyzer.getConfidence();
            logger.logDebug("Geste détecté: " + gesture + " (conf: " + String(confidence) + ")");
            data.labels.push_back(gesture);
        }
    }
}

void handleWiFiData(const SensorData& data) {
    if (millis() - lastWiFiCheck >= WIFI_CHECK_INTERVAL) {
        wifiManager.update();
        lastWiFiCheck = millis();
    }
    
    if (wifiManager.isConnected()) {
        wifiManager.sendData(data);
    }
}

void handleDataLogging(const SensorData& data) {
    if (millis() - lastDataLog >= DATA_LOG_INTERVAL) {
        logger.log(data);
        lastDataLog = millis();
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
        logger.logDebug("Mode changé: " + String(static_cast<int>(currentMode)));
    }
    
    if (M5.BtnB.wasPressed() && millis() - lastButtonCheck >= BUTTON_DELAY) {
        currentDisplay = static_cast<DisplayMode>((static_cast<int>(currentDisplay) + 1) % 6);
        display.setMode(currentDisplay);
        lastButtonCheck = millis();
        logger.logDebug("Affichage changé: " + String(static_cast<int>(currentDisplay)));
    }
    
    if (M5.BtnC.wasPressed() && millis() - lastButtonCheck >= BUTTON_DELAY) {
        if (config.autoLearn) {
            huskyLens.learn(1);
            logger.logDebug("Apprentissage automatique effectué");
        }
        lastButtonCheck = millis();
    }
    
    // Double-clic sur B pour exporter les logs
    static unsigned long lastBClick = 0;
    if (M5.BtnB.wasPressed()) {
        if (millis() - lastBClick < 500) {
            String filename = "/export_" + String(millis()) + ".csv";
            if (logger.exportCSV(filename)) {
                display.showError("Export OK!", true);
            } else {
                display.showError("Export erreur", false);
            }
        }
        lastBClick = millis();
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
    logger.begin();
    configManager.begin();
    configManager.setConfigChangeCallback(onConfigChange);
    
    // Charger la configuration
    config = configManager.loadConfig();
    
    // Initialiser le WiFi si configuré
    if (config.autoLearn) {
        wifiManager.begin();
    }
    
    if (!huskyLens.begin()) {
        display.showError("Erreur HuskyLens!");
        logger.logError("Échec de l'initialisation HuskyLens");
        delay(2000);
    }
    
    display.begin();
    processor.begin();
    
    // Configurer les gestes prédéfinis
    std::vector<Point> circle;
    for (int i = 0; i < 32; i++) {
        float angle = i * 2 * M_PI / 32;
        circle.push_back(Point(cos(angle) * 100, sin(angle) * 100));
    }
    gestureAnalyzer.addPattern("cercle", circle);
    
    std::vector<Point> swipeRight = {
        Point(0, 50), Point(100, 50)
    };
    gestureAnalyzer.addPattern("swipe_right", swipeRight);
    
    std::vector<Point> swipeLeft = {
        Point(100, 50), Point(0, 50)
    };
    gestureAnalyzer.addPattern("swipe_left", swipeLeft);
    
    // Message de démarrage
    display.showError("Système prêt!", true);
    logger.logDebug("Système initialisé avec succès");
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
            
            // Gestion des données
            processor.process(data);
            handleGestures(data);
            handleDataLogging(data);
            handleWiFiData(data);
            
            // Mise à jour de l'affichage
            display.update(processor.getDisplayData());
            
            // Sauvegarder la configuration si nécessaire
            static unsigned long lastConfigSave = 0;
            if (millis() - lastConfigSave >= 60000) { // Toutes les minutes
                configManager.saveConfig(config);
                lastConfigSave = millis();
            }
        } else {
            display.showError("Connexion perdue!");
            logger.logError("Connexion HuskyLens perdue");
        }
    }
    
    delay(20);
}
