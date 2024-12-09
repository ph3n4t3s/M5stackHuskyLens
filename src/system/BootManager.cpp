#include "BootManager.h"

BootManager::BootManager(DisplayManager& display) 
    : display(display),
      currentState(SystemState::BOOTING),
      bootStartTime(0),
      currentStage(0),
      bootComplete(false) {
}

void BootManager::begin() {
    bootStartTime = millis();
    currentStage = 0;
    bootComplete = false;
    
    // Initialiser l'affichage pour la séquence de démarrage
    display.setMode(DisplayMode::BOOT_SEQUENCE);
    
    // Premier message de démarrage
    logBootMessage("Démarrage du système...", TFT_BLUE);
    
    // Commencer la séquence de démarrage
    executeBootSequence();
}

void BootManager::update() {
    if (bootComplete) return;
    
    // Vérifier le timeout
    if (checkTimeout()) {
        handleBootTimeout();
        return;
    }
    
    // Exécuter l'étape courante
    switch (currentStage) {
        case 0:
            if (onHardwareCheck()) {
                advanceToNextStage();
            }
            break;
            
        case 1:
            if (onWiFiSetup()) {
                advanceToNextStage();
            }
            break;
            
        case 2:
            if (onHuskyLensConnect()) {
                advanceToNextStage();
            }
            break;
            
        case 3:
            if (onSystemInit()) {
                advanceToNextStage();
            }
            break;
            
        case 4:
            bootComplete = true;
            display.showBootSequence(BootStage::COMPLETE, "Système prêt !");
            break;
    }
}

bool BootManager::isBootComplete() {
    return bootComplete;
}

// Callbacks par défaut - à surcharger dans la classe principale
bool BootManager::onHardwareCheck() {
    logBootMessage("Vérification du matériel...", TFT_GREEN);
    delay(1000); // Simuler une vérification
    return true;
}

bool BootManager::onWiFiSetup() {
    logBootMessage("Configuration du WiFi...", TFT_YELLOW);
    delay(1000); // Simuler une configuration
    return true;
}

bool BootManager::onHuskyLensConnect() {
    logBootMessage("Connexion à HuskyLens...", TFT_MAGENTA);
    delay(1000); // Simuler une connexion
    return true;
}

bool BootManager::onSystemInit() {
    logBootMessage("Initialisation du système...", TFT_CYAN);
    delay(1000); // Simuler une initialisation
    return true;
}

// Méthodes privées
void BootManager::executeBootSequence() {
    currentStage = 0;
    logBootMessage("Démarrage de la séquence d'initialisation", TFT_WHITE);
}

void BootManager::checkHardware() {
    // Vérification du matériel
    bool m5Ready = M5.begin();
    if (!m5Ready) {
        logBootMessage("Erreur d'initialisation M5Stack", TFT_RED);
        currentState = SystemState::ERROR;
        return;
    }
    
    logBootMessage("M5Stack initialisé avec succès", TFT_GREEN);
}

void BootManager::setupWiFi() {
    // La configuration WiFi sera gérée séparément
    logBootMessage("Configuration WiFi en attente...", TFT_YELLOW);
}

void BootManager::connectHuskyLens() {
    // La connexion HuskyLens sera gérée séparément
    logBootMessage("Tentative de connexion HuskyLens...", TFT_MAGENTA);
}

void BootManager::initializeSystem() {
    // Initialisation finale du système
    logBootMessage("Finalisation de l'initialisation...", TFT_CYAN);
}

void BootManager::logBootMessage(const String& message, uint32_t color) {
    bootLog.push_back(message);
    display.addBootLog(message, color);
    
    // Garder seulement les derniers messages
    while (bootLog.size() > MAX_BOOT_MESSAGES) {
        bootLog.erase(bootLog.begin());
    }
}

void BootManager::advanceToNextStage() {
    currentStage++;
    if (currentStage >= BOOT_STAGES) {
        bootComplete = true;
    }
}

void BootManager::handleBootTimeout() {
    logBootMessage("Timeout durant le démarrage!", TFT_RED);
    currentState = SystemState::ERROR;
}

bool BootManager::checkTimeout() {
    return (millis() - bootStartTime) > (BOOT_STAGES * STAGE_TIMEOUT);
}