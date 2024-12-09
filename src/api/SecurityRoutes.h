#pragma once

#include "WebServer.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "../security/SecurityManager.h"

class SecurityRoutes {
public:
    static void registerRoutes(WebServer::IRouter& router, SecurityManager& securityManager);
    
private:
    struct SecurityResponse {
        bool success;
        String message;
        DynamicJsonDocument data;
        
        SecurityResponse(bool s = true, const String& msg = "", size_t capacity = 1024)
            : success(s), message(msg), data(capacity) {}
    };
    
    static WebServer::RequestCallback makeHandler(
        SecurityManager& manager,
        std::function<SecurityResponse(AsyncWebServerRequest*, SecurityManager&)> handler);
    
    static SecurityResponse handleGetSecurityConfig(AsyncWebServerRequest* request, SecurityManager& manager);
    static SecurityResponse handleUpdateSecurityLevel(AsyncWebServerRequest* request, SecurityManager& manager);
    static SecurityResponse handleUpdateSecurityConfig(AsyncWebServerRequest* request, SecurityManager& manager);
    static SecurityResponse handleAddUser(AsyncWebServerRequest* request, SecurityManager& manager);
    static SecurityResponse handleGetUsers(AsyncWebServerRequest* request, SecurityManager& manager);
    static SecurityResponse handleDeleteUser(AsyncWebServerRequest* request, SecurityManager& manager);
    static SecurityResponse handleRotateKeys(AsyncWebServerRequest* request, SecurityManager& manager);
    static SecurityResponse handleBackup(AsyncWebServerRequest* request, SecurityManager& manager);
    static SecurityResponse handleCheckIntegrity(AsyncWebServerRequest* request, SecurityManager& manager);
    static SecurityResponse handleReset(AsyncWebServerRequest* request, SecurityManager& manager);
    static SecurityResponse handleSecurityLogs(AsyncWebServerRequest* request, SecurityManager& manager);

    // Utilitaires
    static void sendJsonResponse(AsyncWebServerRequest* request, const SecurityResponse& response);
    static bool validateSession(AsyncWebServerRequest* request, SecurityManager& manager);
    static bool checkPermissions(AsyncWebServerRequest* request, SecurityManager& manager, const String& permission);
};