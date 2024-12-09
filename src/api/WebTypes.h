#pragma once

#ifndef WEB_TYPES_H
#define WEB_TYPES_H

#include <Arduino.h>
#include <functional>

// Méthodes HTTP
enum WebRequestMethod {
    HTTP_GET     = 0b00000001,
    HTTP_POST    = 0b00000010,
    HTTP_DELETE  = 0b00000100,
    HTTP_PUT     = 0b00001000,
    HTTP_PATCH   = 0b00010000,
    HTTP_HEAD    = 0b00100000,
    HTTP_OPTIONS = 0b01000000,
    HTTP_ANY     = 0b01111111,
};

// Type de fonction de gestionnaire de requête
using RequestHandler = std::function<void(void*)>;

// Structure pour les en-têtes HTTP
struct WebHeader {
    String name;
    String value;
};

// Structure pour les paramètres de requête
struct WebParameter {
    String name;
    String value;
    bool isFile;
    bool isPost;
};

// Structure pour la réponse
struct WebResponse {
    int code;
    String contentType;
    String content;
    
    WebResponse(int c = 200, const String& type = "text/plain", const String& data = "") 
        : code(c), contentType(type), content(data) {}
};

// Constantes pour les codes de statut HTTP
namespace HttpStatus {
    const int OK = 200;
    const int CREATED = 201;
    const int ACCEPTED = 202;
    const int NO_CONTENT = 204;
    const int BAD_REQUEST = 400;
    const int UNAUTHORIZED = 401;
    const int FORBIDDEN = 403;
    const int NOT_FOUND = 404;
    const int METHOD_NOT_ALLOWED = 405;
    const int INTERNAL_ERROR = 500;
}

// Constantes pour les types de contenu
namespace ContentType {
    const char* const TEXT_PLAIN = "text/plain";
    const char* const TEXT_HTML = "text/html";
    const char* const APPLICATION_JSON = "application/json";
    const char* const APPLICATION_XML = "application/xml";
    const char* const IMAGE_JPEG = "image/jpeg";
    const char* const IMAGE_PNG = "image/png";
}

#endif // WEB_TYPES_H