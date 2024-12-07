#pragma once

#include <ESPAsyncWebServer.h>
#include "../security/SecurityManager.h"

class AuthMiddleware {
public:
    static bool authenticate(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static bool authorizeAdmin(AsyncWebServerRequest* request, SecurityManager& securityManager);
    static bool authorizeResource(AsyncWebServerRequest* request, SecurityManager& securityManager, const String& resource);
    
private:
    static String extractToken(AsyncWebServerRequest* request);
    static SecurityToken parseToken(const String& tokenStr);
};