#pragma once

#include <M5CoreS3.h>
#include "Config.h"
#include "DisplayManager.h"
#include <vector>

class BootManager {
public:
    BootManager(DisplayManager& display);
    
    void begin();
    void update();
    bool isBootComplete();
    
    // Callbacks pour les différentes étapes
    bool onHardwareCheck();
    bool onWiFiSetup();
    bool onHuskyLensConnect();
    bool onSystemInit();
    
private:
    DisplayManager& display;
    SystemState currentState;
    unsigned long bootStartTime;
    int currentStage;
    std::vector<String> bootLog;
    bool bootComplete;
    
    // Étapes de démarrage
    void executeBootSequence();
    void checkHardware();
    void setupWiFi();
    void connectHuskyLens();
    void initializeSystem();
    
    // Utilitaires
    void logBootMessage(const String& message, uint32_t color = TFT_WHITE);
    void advanceToNextStage();
    void handleBootTimeout();
    bool checkTimeout();
    
    // Constantes
    static const int BOOT_STAGES = 5;
    static const unsigned long STAGE_TIMEOUT = 10000; // 10 secondes par étape
};