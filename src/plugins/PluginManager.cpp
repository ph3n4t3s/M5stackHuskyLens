#include "PluginManager.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>

PluginManager::PluginManager() {}

bool PluginManager::begin() {
    if (!SPIFFS.begin(true)) {
        logError("system", "Failed to mount SPIFFS");
        return false;
    }
    
    // Créer le répertoire des plugins si nécessaire
    if (!SPIFFS.exists(m_config.plugin_directory)) {
        SPIFFS.mkdir(m_config.plugin_directory);
    }
    
    // Charger les plugins installés
    File root = SPIFFS.open(m_config.plugin_directory);
    File file = root.openNextFile();
    
    while (file) {
        if (!file.isDirectory() && String(file.name()).endsWith(".json")) {
            String name = String(file.name()).substring(0, String(file.name()).length() - 5);
            loadPluginManifest(name);
        }
        file = root.openNextFile();
    }
    
    // Résoudre les dépendances
    if (m_config.check_dependencies) {
        resolveDependencies();
    }
    
    // Charger les plugins activés
    for (const auto& plugin : m_plugin_info) {
        if (plugin.second.enabled && m_config.auto_enable) {
            loadPlugin(plugin.first);
        }
    }
    
    return true;
}

bool PluginManager::registerPlugin(const String& name, PluginFactory factory, const PluginInfo& info) {
    // Vérifier si le plugin existe déjà
    if (m_plugins.find(name) != m_plugins.end()) {
        logError(name, "Plugin already registered");
        return false;
    }
    
    // Valider le plugin
    if (!validatePlugin(name)) {
        return false;
    }
    
    // Vérifier les dépendances
    if (m_config.check_dependencies && !checkDependencies(name)) {
        logError(name, "Dependencies not satisfied");
        return false;
    }
    
    // Enregistrer le plugin
    m_factories[name] = factory;
    m_plugin_info[name] = info;
    
    // Créer l'instance si auto_enable
    if (info.enabled && m_config.auto_enable) {
        return loadPlugin(name);
    }
    
    return true;
}

bool PluginManager::loadPlugin(const String& name) {
    auto factory_it = m_factories.find(name);
    if (factory_it == m_factories.end()) {
        logError(name, "Plugin factory not found");
        return false;
    }
    
    // Créer l'instance
    try {
        IPlugin* plugin = factory_it->second();
        if (!plugin) {
            logError(name, "Failed to create plugin instance");
            return false;
        }
        
        // Initialiser le plugin
        if (!plugin->init()) {
            delete plugin;
            logError(name, "Plugin initialization failed");
            return false;
        }
        
        // Sandbox si nécessaire
        if (m_config.check_dependencies) {
            sandboxPlugin(name);
        }
        
        m_plugins[name] = plugin;
        return true;
        
    } catch (const std::exception& e) {
        logError(name, String("Plugin creation failed: ") + e.what());
        return false;
    }
}

void PluginManager::sandboxPlugin(const String& name) {
    // TODO: Implémenter l'isolation des plugins
    // - Restrictions d'accès fichiers
    // - Limites de ressources
    // - Isolation mémoire
}

bool PluginManager::checkDependencies(const String& name) {
    auto it = m_plugin_info.find(name);
    if (it == m_plugin_info.end()) return false;
    
    const auto& deps = it->second.dependencies;
    for (const auto& dep : deps) {
        if (m_plugin_info.find(dep) == m_plugin_info.end()) {
            logError(name, "Missing dependency: " + dep);
            return false;
        }
    }
    
    // Vérifier les dépendances circulaires
    std::vector<String> visited;
    if (checkCircularDependencies(name, visited)) {
        logError(name, "Circular dependency detected");
        return false;
    }
    
    return true;
}

bool PluginManager::checkCircularDependencies(const String& name, 
                                            std::vector<String>& visited) {
    if (std::find(visited.begin(), visited.end(), name) != visited.end()) {
        return true;  // Cycle détecté
    }
    
    visited.push_back(name);
    
    auto it = m_plugin_info.find(name);
    if (it == m_plugin_info.end()) return false;
    
    for (const auto& dep : it->second.dependencies) {
        if (checkCircularDependencies(dep, visited)) {
            return true;
        }
    }
    
    visited.pop_back();
    return false;
}

bool PluginManager::hotReload(const String& name) {
    if (!m_config.allow_hot_reload) {
        logError(name, "Hot reload not allowed");
        return false;
    }
    
    // Sauvegarder l'état actuel
    auto plugin = m_plugins[name];
    auto info = m_plugin_info[name];
    
    // Décharger le plugin
    unloadPlugin(name);
    
    // Recharger le plugin
    bool success = loadPlugin(name);
    
    if (!success && plugin) {
        // Restaurer l'ancien état en cas d'échec
        m_plugins[name] = plugin;
        m_plugin_info[name] = info;
    }
    
    return success;
}

void PluginManager::triggerEvent(const String& event, void* data) {
    auto it = m_event_handlers.find(event);
    if (it == m_event_handlers.end()) return;
    
    for (const auto& handler : it->second) {
        try {
            handler(event, data);
        } catch (const std::exception& e) {
            logError("event", String("Event handler failed: ") + e.what());
        }
    }
}

bool PluginManager::loadPluginManifest(const String& name) {
    String path = m_config.plugin_directory + "/" + name + ".json";
    File file = SPIFFS.open(path, "r");
    if (!file) return false;
    
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) {
        logError(name, String("Failed to parse manifest: ") + error.c_str());
        return false;
    }
    
    PluginInfo info;
    info.name = doc["name"] | name;
    info.version = doc["version"] | "1.0.0";
    info.author = doc["author"] | "unknown";
    info.description = doc["description"] | "";
    info.type = static_cast<PluginType>(doc["type"] | 0);
    info.enabled = doc["enabled"] | true;
    
    JsonArray deps = doc["dependencies"];
    for (JsonVariant dep : deps) {
        info.dependencies.push_back(dep.as<String>());
    }
    
    if (!validateManifest(info)) {
        return false;
    }
    
    m_plugin_info[name] = info;
    return true;
}

bool PluginManager::validateManifest(const PluginInfo& info) {
    // Vérifier les champs obligatoires
    if (info.name.isEmpty() || info.version.isEmpty()) {
        return false;
    }
    
    // Vérifier le format de version (semver)
    if (!info.version.matches("^\\d+\\.\\d+\\.\\d+$")) {
        return false;
    }
    
    // Vérifier l'auteur
    if (!info.author.isEmpty() && 
        std::find(m_trusted_authors.begin(), m_trusted_authors.end(), info.author) 
        == m_trusted_authors.end()) {
        return false;
    }
    
    return true;
}

void PluginManager::logError(const String& plugin, const String& error) {
    m_errors.push_back(plugin + ": " + error);
}