#include "ConfigManager.h"
#include <SPIFFS.h>

const char* ConfigManager::PREF_NAMESPACE = "husky_config";
const char* ConfigManager::CONFIG_VERSION_KEY = "config_ver";

ConfigManager::ConfigManager() : onConfigChange(nullptr) {}

bool ConfigManager::begin() {
    if (!preferences.begin(PREF_NAMESPACE, false)) {
        return false;
    }
    
    int configVersion = preferences.getInt(CONFIG_VERSION_KEY, 0);
    if (configVersion < CURRENT_CONFIG_VERSION) {
        migrateConfig(configVersion);
    }
    
    return true;
}

void ConfigManager::saveConfig(const Configuration& config) {
    if (!validateConfig(config)) return;
    
    String configJson = serializeConfig(config);
    preferences.putString("config", configJson);
    preferences.putInt(CONFIG_VERSION_KEY, CURRENT_CONFIG_VERSION);
    
    notifyConfigChange(config);
}

Configuration ConfigManager::loadConfig() {
    String configJson = preferences.getString("config", "");
    if (configJson.isEmpty()) {
        Configuration defaultConfig;
        saveConfig(defaultConfig);
        return defaultConfig;
    }
    
    return deserializeConfig(configJson);
}

void ConfigManager::resetToDefaults() {
    Configuration defaultConfig;
    saveConfig(defaultConfig);
}

bool ConfigManager::exportConfig(const String& filename) {
    if (!SPIFFS.begin(true)) {
        return false;
    }
    
    Configuration config = loadConfig();
    String configJson = serializeConfig(config);
    
    File file = SPIFFS.open(filename, "w");
    if (!file) {
        return false;
    }
    
    size_t written = file.print(configJson);
    file.close();
    
    return written == configJson.length();
}

bool ConfigManager::importConfig(const String& filename) {
    if (!SPIFFS.begin(true)) {
        return false;
    }
    
    if (!SPIFFS.exists(filename)) {
        return false;
    }
    
    File file = SPIFFS.open(filename, "r");
    if (!file) {
        return false;
    }
    
    String configJson = file.readString();
    file.close();
    
    Configuration config = deserializeConfig(configJson);
    if (!validateConfig(config)) {
        return false;
    }
    
    saveConfig(config);
    return true;
}

void ConfigManager::setConfigChangeCallback(void (*callback)(const Configuration&)) {
    onConfigChange = callback;
}

bool ConfigManager::validateConfig(const Configuration& config) {
    if (config.sensitivity < 1 || config.sensitivity > 10) return false;
    if (config.zoomLevel < 1 || config.zoomLevel > 5) return false;
    if (config.recordingInterval < 100 || config.recordingInterval > 10000) return false;
    
    return true;
}

void ConfigManager::migrateConfig(int fromVersion) {
    Configuration config = loadConfig();
    
    // Migration de la version 0 vers 1
    if (fromVersion == 0) {
        // Initialisation des nouveaux champs
        config.nightMode = false;
        config.audioFeedback = true;
        fromVersion = 1;
    }
    
    // Migration de la version 1 vers 2 (future)
    if (fromVersion == 1) {
        // Prêt pour les futures migrations
    }
    
    saveConfig(config);
}

String ConfigManager::serializeConfig(const Configuration& config) {
    DynamicJsonDocument doc(1024);
    
    doc["sensitivity"] = config.sensitivity;
    doc["zoomLevel"] = config.zoomLevel;
    doc["autoLearn"] = config.autoLearn;
    doc["audioFeedback"] = config.audioFeedback;
    doc["nightMode"] = config.nightMode;
    doc["recordingInterval"] = config.recordingInterval;
    
    String output;
    serializeJson(doc, output);
    return output;
}

Configuration ConfigManager::deserializeConfig(const String& json) {
    Configuration config;
    DynamicJsonDocument doc(1024);
    
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        return config; // Retourne la configuration par défaut en cas d'erreur
    }
    
    config.sensitivity = doc["sensitivity"] | 3;
    config.zoomLevel = doc["zoomLevel"] | 1;
    config.autoLearn = doc["autoLearn"] | false;
    config.audioFeedback = doc["audioFeedback"] | true;
    config.nightMode = doc["nightMode"] | false;
    config.recordingInterval = doc["recordingInterval"] | 1000;
    
    return config;
}

void ConfigManager::notifyConfigChange(const Configuration& config) {
    if (onConfigChange) {
        onConfigChange(config);
    }
}