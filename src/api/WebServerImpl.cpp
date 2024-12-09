#include "WebServer.h"
#include <functional>
#include <memory>
#include <vector>

namespace {
    // Instance unique du serveur web
    std::unique_ptr<AsyncWebServer> server;
    
    // Configuration actuelle
    WebServer::Config currentConfig;
    
    // Liste des gestionnaires de routes
    struct RouteHandler {
        const char* path;
        WebRequestMethod method;
        ArRequestHandlerFunction handler;
    };
    
    std::vector<RouteHandler> routeHandlers;
}

namespace WebServer {
    bool begin(const Config& config) {
        if (server) {
            return false; // Déjà initialisé
        }
        
        currentConfig = config;
        server.reset(new AsyncWebServer(config.port));
        
        // Appliquer les gestionnaires de routes enregistrés
        for (const auto& route : routeHandlers) {
            server->on(route.path, route.method, route.handler);
        }
        
        server->begin();
        return true;
    }
    
    void stop() {
        if (server) {
            server->end();
            server.reset();
        }
    }
    
    void addRoute(const char* path, WebRequestMethod method, ArRequestHandlerFunction handler) {
        routeHandlers.push_back({path, method, handler});
        if (server) {
            server->on(path, method, handler);
        }
    }
    
    void setNotFoundHandler(ArRequestHandlerFunction handler) {
        if (server) {
            server->onNotFound(handler);
        }
    }
    
    bool isRunning() {
        return server != nullptr;
    }
    
    const Config& getConfig() {
        return currentConfig;
    }
}