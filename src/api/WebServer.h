#pragma once

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "WebTypes.h"
#include <vector>
#include <memory>
#include <functional>
#include <ArduinoJson.h>

// Forward declarations
class AsyncWebServer;
class AsyncWebSocket;
class AsyncEventSource;

#ifdef ARDUINO_ARCH_ESP32
    #if defined(ESP_ASYNC_WEBSERVER)
        #include <ESPAsyncWebServer.h>
        #include <AsyncTCP.h>
    #else 
        #if __has_include("AsyncWebServer_ESPAsyncWebServer.h")
            #include <AsyncWebServer_ESPAsyncWebServer.h>
            #include <AsyncTCP.h>
        #else
            #error "ESPAsyncWebServer library not found! Please install ottowinter/ESPAsyncWebServer-esphome"
        #endif
    #endif
#else
    #error "This library only supports ESP32 boards"
#endif

// Interface principale du serveur web
namespace WebServer {
    // Types et structures
    using RequestCallback = std::function<void(AsyncWebServerRequest*)>;
    using BodyCallback = std::function<void(AsyncWebServerRequest*, uint8_t*, size_t, size_t, size_t)>;
    using UploadCallback = std::function<void(AsyncWebServerRequest*, String, size_t, uint8_t*, size_t, bool)>;
    
    struct Route {
        String path;
        WebRequestMethod method;
        RequestCallback handler;
        BodyCallback bodyHandler;
        UploadCallback uploadHandler;
        bool isSecure;
        
        Route(const String& p, WebRequestMethod m, RequestCallback h, 
              BodyCallback b = nullptr, UploadCallback u = nullptr, bool secure = false)
            : path(p), method(m), handler(h), bodyHandler(b), 
              uploadHandler(u), isSecure(secure) {}
    };
    
    struct Config {
        uint16_t port;
        bool secureConnection;
        String hostname;
        size_t maxRequestSize;
        bool corsEnabled;
        String corsOrigin;
        
        Config(uint16_t p = 80) 
            : port(p)
            , secureConnection(false)
            , hostname("m5stack")
            , maxRequestSize(16384)
            , corsEnabled(true)
            , corsOrigin("*") {}
    };

    // Interface pour la gestion des routes
    class IRouter {
    public:
        virtual ~IRouter() = default;
        virtual void addRoute(const Route& route) = 0;
        virtual void removeRoute(const String& path) = 0;
        virtual void setErrorHandler(RequestCallback handler) = 0;
        virtual void setNotFoundHandler(RequestCallback handler) = 0;
    };

    // Interface pour la gestion des WebSockets
    class IWebSocketHandler {
    public:
        virtual ~IWebSocketHandler() = default;
        virtual void onConnect(std::function<void(AsyncWebSocket*, AsyncWebSocketClient*)> cb) = 0;
        virtual void onDisconnect(std::function<void(AsyncWebSocket*, AsyncWebSocketClient*)> cb) = 0;
        virtual void onMessage(std::function<void(AsyncWebSocket*, AsyncWebSocketClient*, String)> cb) = 0;
        virtual void broadcast(const String& message) = 0;
    };

    // Interface du serveur
    class IServer {
    public:
        virtual ~IServer() = default;
        virtual bool start() = 0;
        virtual void stop() = 0;
        virtual IRouter* router() = 0;
        virtual IWebSocketHandler* ws(const String& path) = 0;
        virtual void enableCORS(bool enable, const String& origin = "*") = 0;
        virtual bool isRunning() const = 0;
        virtual const Config& config() const = 0;
    };

    // Fonction de création du serveur
    std::unique_ptr<IServer> createServer(const Config& config = Config());
}

#endif // WEBSERVER_H