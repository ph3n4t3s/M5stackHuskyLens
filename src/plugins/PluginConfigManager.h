#pragma once

#include <ArduinoJson.h>
#include <map>
#include <vector>
#include "PluginManager.h"

// Structure pour les paramètres de configuration
struct ConfigParam {
    String name;
    String description;
    enum class Type {
        BOOL,
        INT,
        FLOAT,
        STRING,
        ENUM,
        ARRAY,
        OBJECT
    } type;
    
    union Value {
        bool b;
        int i;
        float f;
        void* ptr;
        
        Value() : ptr(nullptr) {}
        Value(bool v) : b(v) {}
        Value(int v) : i(v) {}
        Value(float v) : f(v) {}
        Value(void* v) : ptr(v) {}
    } defaultValue;
    
    std::vector<String> enumOptions;
    String units;
    float min;
    float max;
    float step;
    bool required;
    std::vector<String> dependencies;
    String validationRegex;
    
    ConfigParam() : 
        type(Type::STRING),
        min(0),
        max(0),
        step(0),
        required(false) {}
};

// Structure pour la définition de l'interface
struct PluginUI {
    String title;
    String description;
    std::vector<String> categories;
    std::vector<std::pair<String, std::vector<ConfigParam>>> sections;
    std::map<String, String> help;
    std::vector<String> previewFields;
};

class PluginConfigManager {
public:
    PluginConfigManager(PluginManager& pluginManager);
    bool begin();
    
    // Gestion des configurations
    bool registerConfig(const String& pluginName, const std::vector<ConfigParam>& params);
    bool unregisterConfig(const String& pluginName);
    bool updateConfig(const String& pluginName, const String& paramName, const String& value);
    bool resetConfig(const String& pluginName);
    
    // Interface utilisateur
    bool registerUI(const String& pluginName, const PluginUI& ui);
    PluginUI getUI(const String& pluginName) const;
    String generateHTML(const String& pluginName) const;
    String generateJSON(const String& pluginName) const;
    
    // Validation et persistance
    bool validateConfig(const String& pluginName, const JsonObject& config);
    bool loadConfig(const String& pluginName);
    bool saveConfig(const String& pluginName);
    bool exportConfig(const String& filename);
    bool importConfig(const String& filename);
    
    // Préréglages
    bool savePreset(const String& name, const String& pluginName);
    bool loadPreset(const String& name, const String& pluginName);
    std::vector<String> listPresets(const String& pluginName) const;
    
    // Événements et notifications
    using ConfigChangeCallback = std::function<void(const String& plugin, 
                                                  const String& param,
                                                  const String& value)>;
    void onConfigChange(ConfigChangeCallback callback);
    
    // Utilitaires
    std::vector<String> getConfigurablePlugins() const;
    bool hasConfig(const String& pluginName) const;
    JsonObject getConfig(const String& pluginName);

private:
    PluginManager& m_pluginManager;
    std::map<String, std::vector<ConfigParam>> m_configs;
    std::map<String, PluginUI> m_uis;
    std::map<String, JsonObject> m_values;
    std::vector<ConfigChangeCallback> m_callbacks;
    
    // Validation
    bool validateParam(const ConfigParam& param, const JsonVariant& value);
    bool checkDependencies(const String& pluginName, const String& paramName);
    String getValidationError(const ConfigParam& param, const JsonVariant& value);
    
    // Gestion de l'interface
    String generateFormField(const ConfigParam& param, const String& value) const;
    String generateValidationJS(const String& pluginName) const;
    String generateDependencyJS(const String& pluginName) const;
    
    // Persistance
    String getConfigPath(const String& pluginName) const;
    String getPresetPath(const String& name, const String& pluginName) const;
    bool createConfigDirectories();
    
    // Utilitaires
    JsonVariant convertValue(const ConfigParam& param, const String& value);
    String convertToString(const ConfigParam& param, const JsonVariant& value);
    void notifyConfigChange(const String& plugin, const String& param, const String& value);
};