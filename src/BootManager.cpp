#include "BootManager.h"

BootManager::BootManager(DisplayManager& display) 
    : displayManager(display), bootComplete(false), bootStartTime(0) {}

void BootManager::begin() {
    bootStartTime = millis();
    bootComplete = false;
    bootMessages.clear();
    
    initializeHardware();
}

void BootManager::update() {
    if (bootComplete) return;
    
    if (!bootMessages.empty()) {
        auto& lastMsg = bootMessages.back();
        displayManager.showBootSequence(lastMsg.message);
    }
    
    if (millis() - bootStartTime > 5000) { // 5 secondes de délai de démarrage
        finalizeBootSequence();
    }
}

bool BootManager::isBootComplete() const {
    return bootComplete;
}

void BootManager::showBootSequence(const String& message) {
    bootMessages.emplace_back(message, Constants::Colors::INFO);
    displayManager.showBootSequence(message);
}

void BootManager::initializeHardware() {
    showBootSequence("Initialisation du matériel...");
    // Ajouter ici l'initialisation du matériel
    
    checkDependencies();
}

void BootManager::checkDependencies() {
    showBootSequence("Vérification des dépendances...");
    // Ajouter ici la vérification des dépendances
    
    finalizeBootSequence();
}

void BootManager::finalizeBootSequence() {
    bootComplete = true;
    showBootSequence("Démarrage terminé");
}