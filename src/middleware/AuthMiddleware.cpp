#include "AuthMiddleware.h"
#include <ArduinoJson.h>

bool AuthMiddleware::authenticate(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    String tokenStr = extractToken(request);
    if (tokenStr.isEmpty()) {
        return false;
    }
    
    SecurityToken token = parseToken(tokenStr);
    return securityManager.validateToken(token);
}

bool AuthMiddleware::authorizeAdmin(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    String tokenStr = extractToken(request);
    if (tokenStr.isEmpty()) {
        return false;
    }
    
    SecurityToken token = parseToken(tokenStr);
    return securityManager.checkPermission(token, "admin");
}

bool AuthMiddleware::authorizeResource(AsyncWebServerRequest* request, 
                                    SecurityManager& securityManager, 
                                    const String& resource) {
    String tokenStr = extractToken(request);
    if (tokenStr.isEmpty()) {
        return false;
    }
    
    SecurityToken token = parseToken(tokenStr);
    return securityManager.checkPermission(token, resource);
}

String AuthMiddleware::extractToken(AsyncWebServerRequest* request) {
    if (!request->hasHeader("Authorization")) {
        return "";
    }
    
    String auth = request->header("Authorization");
    if (!auth.startsWith("Bearer ")) {
        return "";
    }
    
    return auth.substring(7);  // Enlever "Bearer "
}

SecurityToken AuthMiddleware::parseToken(const String& tokenStr) {
    SecurityToken token;
    
    // Format attendu: base64(header.payload.signature)
    int firstDot = tokenStr.indexOf('.');
    int lastDot = tokenStr.lastIndexOf('.');
    
    if (firstDot == -1 || lastDot == -1 || firstDot == lastDot) {
        return token;
    }
    
    String payloadBase64 = tokenStr.substring(firstDot + 1, lastDot);
    
    // Décoder le payload
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payloadBase64);
    
    if (error) {
        return token;
    }
    
    token.token = tokenStr;
    token.userID = doc["sub"].as<String>();
    token.expiry = doc["exp"].as<unsigned long>();
    
    JsonArray perms = doc["permissions"].as<JsonArray>();
    for (JsonVariant perm : perms) {
        token.permissions.push_back(perm.as<String>());
    }
    
    return token;
}