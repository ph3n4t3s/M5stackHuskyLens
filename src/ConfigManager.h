#pragma once

#include "Types.h"
#include <Preferences.h>

class ConfigManager {
public:
    ConfigManager();

    // Initialisation
    void begin();
    
    // Configuration WiFi
    void setWiFiCredentials(const char* ssid, const char* password);
    String getWiFiSSID() const;
    String getWiFiPassword() const;
    bool isWiFiConfigured() const;
    
    // Configuration système
    uint8_t getBrightness() const;
    void setBrightness(uint8_t value);
    void saveCurrentState();
    
    // État du système
    SystemConfig systemConfig;

private:
    Preferences prefs;
    bool initialized;
    
    void loadConfig();
    void saveConfig();
};