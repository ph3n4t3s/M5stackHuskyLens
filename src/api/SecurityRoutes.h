#pragma once

#include <ESPAsyncWebServer.h>
#include "../security/SecurityManager.h"

class SecurityRoutes {
public:
    static void registerRoutes(AsyncWebServer& server, SecurityManager& securityManager);
    
private:
    static void handleGetSecurityConfig(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static void handleUpdateSecurityLevel(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static void handleUpdateSecurityConfig(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static void handleAddUser(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static void handleGetUsers(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static void handleDeleteUser(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static void handleRotateKeys(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static void handleBackup(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static void handleCheckIntegrity(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static void handleReset(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static void handleSecurityLogs(AsyncWebServerRequest* request, SecurityManager& securityManager);
    
    static void sendJsonResponse(AsyncWebServerRequest* request, bool success, const String& message);
};