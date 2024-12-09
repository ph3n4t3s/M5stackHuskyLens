#pragma once

#ifndef WEBSERVER_H
#define WEBSERVER_H

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

// Configuration par défaut du serveur web
#ifndef WEBSERVER_TASK_PRIORITY
    #define WEBSERVER_TASK_PRIORITY 2
#endif

#ifndef WEBSERVER_STACK_SIZE
    #define WEBSERVER_STACK_SIZE 10240
#endif

// Forward declarations des classes principales
class AsyncWebServer;
class AsyncWebServerRequest;
class AsyncWebParameter;
class AsyncWebHeader;
class AsyncWebSocket;
class AsyncEventSource;

namespace WebServer {
    // Class de configuration
    struct Config {
        uint16_t port;
        bool secureConnection;
        String hostname;
        
        Config(uint16_t p = 80) : 
            port(p),
            secureConnection(false),
            hostname("m5stack") {}
    };
    
    // Constantes pour les codes de retour HTTP
    enum HttpCode {
        OK = 200,
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        SERVER_ERROR = 500
    };
}

#endif // WEBSERVER_H