#pragma once

#include <Preferences.h>
#include <ArduinoJson.h>
#include "Config.h"

class ConfigManager {
public:
    ConfigManager();
    bool begin();
    void saveConfig(const Configuration& config);
    Configuration loadConfig();
    void resetToDefaults();
    bool exportConfig(const String& filename);
    bool importConfig(const String& filename);
    void setConfigChangeCallback(void (*callback)(const Configuration&));
    
private:
    Preferences preferences;
    void (*onConfigChange)(const Configuration&);
    
    static const char* PREF_NAMESPACE;
    static const char* CONFIG_VERSION_KEY;
    static const int CURRENT_CONFIG_VERSION = 1;
    
    bool validateConfig(const Configuration& config);
    void migrateConfig(int fromVersion);
    String serializeConfig(const Configuration& config);
    Configuration deserializeConfig(const String& json);
    void notifyConfigChange(const Configuration& config);
};