#pragma once

#include <vector>
#include <map>
#include <functional>
#include "../Config.h"

// Types de plugins
enum class PluginType {
    DETECTION,        // Algorithmes de détection
    PROCESSING,       // Traitement d'image
    VISUALIZATION,    // Visualisation des données
    ANALYTICS,        // Analyse des données
    COMMUNICATION,    // Communication avec d'autres systèmes
    CUSTOM           // Plugins personnalisés
};

// Interface de base pour les plugins
class IPlugin {
public:
    virtual ~IPlugin() = default;
    
    virtual const char* getName() const = 0;
    virtual const char* getVersion() const = 0;
    virtual PluginType getType() const = 0;
    virtual bool init() = 0;
    virtual void update() = 0;
    virtual void cleanup() = 0;
};

// Structure pour les informations de plugin
struct PluginInfo {
    String name;
    String version;
    String author;
    String description;
    PluginType type;
    bool enabled;
    std::vector<String> dependencies;
    
    PluginInfo() : enabled(true) {}
};

// Configuration du gestionnaire de plugins
struct PluginConfig {
    bool auto_enable;
    bool check_dependencies;
    bool allow_hot_reload;
    String plugin_directory;
    std::vector<String> blacklist;
    std::vector<String> trusted_authors;
    
    PluginConfig() :
        auto_enable(true),
        check_dependencies(true),
        allow_hot_reload(true),
        plugin_directory("/plugins") {}
};

using PluginFactory = std::function<IPlugin*()>;

class PluginManager {
public:
    PluginManager();
    bool begin();
    
    // Gestion des plugins
    bool registerPlugin(const String& name, PluginFactory factory, const PluginInfo& info);
    bool unregisterPlugin(const String& name);
    bool enablePlugin(const String& name, bool enable = true);
    bool reloadPlugin(const String& name);
    
    // Configuration
    void setConfig(const PluginConfig& config);
    PluginConfig getConfig() const;
    
    // Accès aux plugins
    IPlugin* getPlugin(const String& name);
    std::vector<PluginInfo> getPluginList() const;
    std::vector<PluginInfo> getPluginsByType(PluginType type) const;
    
    // Événements et hooks
    using EventCallback = std::function<void(const String& event, void* data)>;
    void registerEventHandler(const String& event, EventCallback callback);
    void unregisterEventHandler(const String& event);
    void triggerEvent(const String& event, void* data = nullptr);
    
    // Gestion des dépendances
    bool checkDependencies(const String& name);
    bool resolveDependencies();
    std::vector<String> getMissingDependencies(const String& name) const;
    
    // Hot reload et mise à jour
    bool hotReload(const String& name);
    bool updatePlugin(const String& name, const String& version);
    void scanForUpdates();
    
    // Sécurité et validation
    bool validatePlugin(const String& name);
    bool isPluginTrusted(const String& name) const;
    void setTrustedAuthors(const std::vector<String>& authors);
    
    // API extension
    bool addPluginAPI(const String& name, void* api);
    void* getPluginAPI(const String& name);
    bool removePluginAPI(const String& name);
    
    // État et diagnostics
    bool isPluginEnabled(const String& name) const;
    String getPluginStatus(const String& name) const;
    std::vector<String> getPluginErrors() const;

private:
    PluginConfig m_config;
    std::map<String, IPlugin*> m_plugins;
    std::map<String, PluginInfo> m_plugin_info;
    std::map<String, PluginFactory> m_factories;
    std::map<String, std::vector<EventCallback>> m_event_handlers;
    std::map<String, void*> m_plugin_apis;
    std::vector<String> m_trusted_authors;
    std::vector<String> m_errors;
    
    // Gestion interne
    bool loadPluginManifest(const String& name);
    bool validateManifest(const PluginInfo& info);
    void sortPluginsByDependencies();
    bool checkCircularDependencies(const String& name, std::vector<String>& visited);
    
    // Installation et chargement
    bool installPlugin(const String& path);
    bool loadPlugin(const String& name);
    void unloadPlugin(const String& name);
    
    // Sécurité
    bool verifyPluginSignature(const String& name);
    bool checkPluginPermissions(const String& name);
    void sandboxPlugin(const String& name);
    
    // Gestion des erreurs
    void logError(const String& plugin, const String& error);
    void clearErrors(const String& plugin);
    
    // Utilitaires
    String getPluginPath(const String& name) const;
    bool isPluginBlacklisted(const String& name) const;
    void cleanupOrphanedData();
};