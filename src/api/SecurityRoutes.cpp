#include "SecurityRoutes.h"
#include <ArduinoJson.h>

void SecurityRoutes::registerRoutes(WebServer::IRouter& router, SecurityManager& securityManager) {
    // Points d'entrée API sécurité
    server.on("/api/security/config", HTTP_GET, 
        [&](AsyncWebServerRequest* request) {
            handleGetSecurityConfig(request, securityManager);
        }
    );
    
    server.on("/api/security/level", HTTP_POST,
        [&](AsyncWebServerRequest* request) {
            handleUpdateSecurityLevel(request, securityManager);
        }
    );
    
    server.on("/api/security/config", HTTP_POST,
        [&](AsyncWebServerRequest* request) {
            handleUpdateSecurityConfig(request, securityManager);
        }
    );
    
    server.on("/api/security/users", HTTP_GET,
        [&](AsyncWebServerRequest* request) {
            handleGetUsers(request, securityManager);
        }
    );
    
    server.on("/api/security/users", HTTP_POST,
        [&](AsyncWebServerRequest* request) {
            handleAddUser(request, securityManager);
        }
    );
    
    server.on("/api/security/users/delete", HTTP_POST,
        [&](AsyncWebServerRequest* request) {
            handleDeleteUser(request, securityManager);
        }
    );
    
    server.on("/api/security/rotate-keys", HTTP_POST,
        [&](AsyncWebServerRequest* request) {
            handleRotateKeys(request, securityManager);
        }
    );
    
    server.on("/api/security/backup", HTTP_GET,
        [&](AsyncWebServerRequest* request) {
            handleBackup(request, securityManager);
        }
    );
    
    server.on("/api/security/check-integrity", HTTP_GET,
        [&](AsyncWebServerRequest* request) {
            handleCheckIntegrity(request, securityManager);
        }
    );
    
    server.on("/api/security/reset", HTTP_POST,
        [&](AsyncWebServerRequest* request) {
            handleReset(request, securityManager);
        }
    );
    
    server.on("/api/security/logs", HTTP_GET,
        [&](AsyncWebServerRequest* request) {
            handleSecurityLogs(request, securityManager);
        }
    );
}

void SecurityRoutes::handleGetSecurityConfig(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    // Vérifier l'authentification
    if (!request->hasHeader("Authorization")) {
        request->send(401, "application/json", "{\"success\":false,\"message\":\"Non authentifié\"}");
        return;
    }
    
    DynamicJsonDocument doc(1024);
    SecurityConfig config; // À implémenter: obtenir la config actuelle
    
    doc["success"] = true;
    doc["enableEncryption"] = config.enableEncryption;
    doc["enableAuthentication"] = config.enableAuthentication;
    doc["enableSecureBootload"] = config.enableSecureBootload;
    doc["enableTLS"] = config.enableTLS;
    doc["keyRotationInterval"] = config.keyRotationInterval;
    doc["sessionTimeout"] = config.sessionTimeout;
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

void SecurityRoutes::handleUpdateSecurityLevel(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    if (!request->hasParam("level", true)) {
        sendJsonResponse(request, false, "Niveau de sécurité manquant");
        return;
    }
    
    String level = request->getParam("level", true)->value();
    SecurityLevel secLevel;
    
    if (level == "LOW") secLevel = SecurityLevel::SEC_LOW;
    else if (level == "MEDIUM") secLevel = SecurityLevel::SEC_MEDIUM;
    else if (level == "HIGH") secLevel = SecurityLevel::SEC_HIGH;
    else if (level == "CUSTOM") secLevel = SecurityLevel::SEC_CUSTOM;
    else {
        sendJsonResponse(request, false, "Niveau de sécurité invalide");
        return;
    }
    
    securityManager.setSecurityLevel(secLevel);
    sendJsonResponse(request, true, "Niveau de sécurité mis à jour");
}

void SecurityRoutes::handleUpdateSecurityConfig(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    if (!request->hasParam("config", true)) {
        sendJsonResponse(request, false, "Configuration manquante");
        return;
    }
    
    String configJson = request->getParam("config", true)->value();
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, configJson);
    
    if (error) {
        sendJsonResponse(request, false, "JSON invalide");
        return;
    }
    
    SecurityConfig config;
    config.enableEncryption = doc["enableEncryption"] | true;
    config.enableAuthentication = doc["enableAuthentication"] | true;
    config.enableSecureBootload = doc["enableSecureBootload"] | false;
    config.enableTLS = doc["enableTLS"] | true;
    config.keyRotationInterval = doc["keyRotationInterval"] | 24*3600;
    config.sessionTimeout = doc["sessionTimeout"] | 3600;
    
    securityManager.setCustomConfig(config);
    sendJsonResponse(request, true, "Configuration mise à jour");
}

void SecurityRoutes::handleAddUser(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    if (!request->hasParam("username", true) || !request->hasParam("password", true)) {
        sendJsonResponse(request, false, "Paramètres manquants");
        return;
    }
    
    String username = request->getParam("username", true)->value();
    String password = request->getParam("password", true)->value();
    
    if (username.length() < 3 || password.length() < 8) {
        sendJsonResponse(request, false, "Username ou password trop court");
        return;
    }
    
    if (securityManager.authenticate(username, password)) {
        sendJsonResponse(request, true, "Utilisateur ajouté");
    } else {
        sendJsonResponse(request, false, "Erreur lors de l'ajout");
    }
}

void SecurityRoutes::handleGetUsers(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    DynamicJsonDocument doc(2048);
    JsonArray users = doc.createNestedArray("users");
    
    // TODO: Implémenter la récupération des utilisateurs
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

void SecurityRoutes::handleRotateKeys(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    securityManager.rotateKeys();
    sendJsonResponse(request, true, "Clés rotées avec succès");
}

void SecurityRoutes::handleBackup(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    if (securityManager.backupSecurityData("/backup.bin")) {
        // Envoyer le fichier
        request->send(SPIFFS, "/backup.bin", "application/octet-stream");
    } else {
        sendJsonResponse(request, false, "Erreur de backup");
    }
}

void SecurityRoutes::handleCheckIntegrity(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    bool integrity = securityManager.checkIntegrity();
    sendJsonResponse(request, integrity, 
        integrity ? "Intégrité vérifiée" : "Problème d'intégrité détecté");
}

void SecurityRoutes::handleReset(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    // TODO: Implémenter la réinitialisation
    sendJsonResponse(request, true, "Système réinitialisé");
}

void SecurityRoutes::handleSecurityLogs(AsyncWebServerRequest* request, SecurityManager& securityManager) {
    auto logs = securityManager.getSecurityLogs();
    
    DynamicJsonDocument doc(4096);
    JsonArray logsArray = doc.createNestedArray("logs");
    
    for (const auto& log : logs) {
        logsArray.add(log);
    }
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

void SecurityRoutes::sendJsonResponse(AsyncWebServerRequest* request, bool success, const String& message) {
    DynamicJsonDocument doc(256);
    doc["success"] = success;
    doc["message"] = message;
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}