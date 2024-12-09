#include "ConfigManager.h"

ConfigManager::ConfigManager() : initialized(false) {}

void ConfigManager::begin() {
    if (!initialized) {
        prefs.begin("m5husky", false);
        loadConfig();
        initialized = true;
    }
}

void ConfigManager::setWiFiCredentials(const char* ssid, const char* password) {
    if (!initialized) return;
    strncpy(systemConfig.wifi.ssid, ssid, sizeof(systemConfig.wifi.ssid) - 1);
    strncpy(systemConfig.wifi.password, password, sizeof(systemConfig.wifi.password) - 1);
    saveConfig();
}

String ConfigManager::getWiFiSSID() const {
    return String(systemConfig.wifi.ssid);
}

String ConfigManager::getWiFiPassword() const {
    return String(systemConfig.wifi.password);
}

bool ConfigManager::isWiFiConfigured() const {
    return strlen(systemConfig.wifi.ssid) > 0;
}

uint8_t ConfigManager::getBrightness() const {
    return systemConfig.display.brightness;
}

void ConfigManager::setBrightness(uint8_t value) {
    systemConfig.display.brightness = value;
}

void ConfigManager::saveCurrentState() {
    saveConfig();
}

void ConfigManager::loadConfig() {
    size_t configSize = prefs.getBytesLength("sys_config");
    if (configSize == sizeof(SystemConfig)) {
        prefs.getBytes("sys_config", &systemConfig, sizeof(SystemConfig));
    }
}

void ConfigManager::saveConfig() {
    if (!initialized) return;
    prefs.putBytes("sys_config", &systemConfig, sizeof(SystemConfig));
}