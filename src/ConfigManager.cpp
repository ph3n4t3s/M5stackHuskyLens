#include "ConfigManager.h"
#include <EEPROM.h>

// Initialisation des variables statiques
WifiConfig ConfigManager::wifiConfig;
DisplayConfig ConfigManager::displayConfig;
SystemConfig ConfigManager::systemConfig;

ConfigManager::ConfigManager() {
    begin();
}

void ConfigManager::begin() {
    EEPROM.begin(512);
    loadAllConfigs();
    
    // Vérification de la première exécution
    if (systemConfig.firstBoot) {
        resetToDefaults();
    }
}

void ConfigManager::setWiFiCredentials(const char* ssid, const char* password) {
    strncpy(wifiConfig.ssid, ssid, sizeof(wifiConfig.ssid) - 1);
    strncpy(wifiConfig.password, password, sizeof(wifiConfig.password) - 1);
    wifiConfig.configured = true;
    wifiConfig.save();
}

bool ConfigManager::isWiFiConfigured() {
    return wifiConfig.configured;
}

void ConfigManager::setDisplayMode(bool nightMode) {
    displayConfig.nightMode = nightMode;
    displayConfig.save();
}

void ConfigManager::setBrightness(uint8_t brightness) {
    displayConfig.brightness = brightness;
    displayConfig.save();
}

void ConfigManager::setDebugMode(bool enabled) {
    systemConfig.debugMode = enabled;
    systemConfig.save();
}

String ConfigManager::getWiFiSSID() {
    return String(wifiConfig.ssid);
}

String ConfigManager::getWiFiPassword() {
    return String(wifiConfig.password);
}

bool ConfigManager::getNightMode() {
    return displayConfig.nightMode;
}

uint8_t ConfigManager::getBrightness() {
    return displayConfig.brightness;
}

void ConfigManager::resetWiFiConfig() {
    memset(&wifiConfig, 0, sizeof(WifiConfig));
    wifiConfig.configured = false;
    wifiConfig.save();
}

void ConfigManager::factoryReset() {
    resetToDefaults();
    resetWiFiConfig();
}

String ConfigManager::getDeviceName() {
    return String(systemConfig.deviceName);
}

void ConfigManager::setDeviceName(const char* name) {
    strncpy(systemConfig.deviceName, name, sizeof(systemConfig.deviceName) - 1);
    systemConfig.save();
}

void ConfigManager::setVolume(uint8_t volume) {
    systemConfig.volume = volume;
    systemConfig.save();
}

uint8_t ConfigManager::getVolume() {
    return systemConfig.volume;
}

bool ConfigManager::getDebugMode() {
    return systemConfig.debugMode;
}

void ConfigManager::saveCurrentState() {
    saveAllConfigs();
}

// Méthodes privées
void ConfigManager::loadAllConfigs() {
    wifiConfig.load();
    displayConfig.load();
    systemConfig.load();
}

void ConfigManager::resetToDefaults() {
    // Configuration de l'affichage
    displayConfig = DEFAULT_DISPLAY_CONFIG;
    displayConfig.save();

    // Configuration système
    systemConfig = DEFAULT_SYSTEM_CONFIG;
    systemConfig.firstBoot = false;
    systemConfig.save();
}