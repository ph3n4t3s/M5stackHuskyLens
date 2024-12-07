#include "WiFiManager.h"

WiFiManager::WiFiManager() : server(80), apMode(false), connected(false) {}

bool WiFiManager::begin(const char* ssid, const char* password) {
    if (!SPIFFS.begin(true)) {
        return false;
    }
    
    if (ssid && password) {
        currentSSID = ssid;
        currentPassword = password;
    } else {
        loadCredentials();
    }
    
    if (currentSSID.length() > 0) {
        startClient();
    } else {
        startAP();
    }
    
    setupWebServer();
    return true;
}

void WiFiManager::startAP() {
    WiFi.mode(WIFI_AP);
    String apName = "HuskyLens_" + String((uint32_t)ESP.getEfuseMac(), HEX);
    WiFi.softAP(apName.c_str(), "huskyconfig");
    apMode = true;
    
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

void WiFiManager::startClient() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(currentSSID.c_str(), currentPassword.c_str());
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        connected = true;
        Serial.println("\nConnecté à " + currentSSID);
        Serial.println("IP: " + WiFi.localIP().toString());
    } else {
        startAP();
    }
}

void WiFiManager::setupWebServer() {
    // Page d'accueil
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    
    // API pour la configuration
    server.on("/api/config", HTTP_GET, [this](AsyncWebServerRequest *request){
        DynamicJsonDocument doc(1024);
        doc["ssid"] = currentSSID;
        doc["connected"] = connected;
        
        String response;
        serializeJson(doc, response);
        request->send(200, "application/json", response);
    });
    
    // API pour recevoir les données en temps réel
    server.on("/api/data", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send(200, "text/event-stream", "retry: 10000\n");
    });
    
    // Configuration WiFi
    server.on("/api/wifi", HTTP_POST, [this](AsyncWebServerRequest *request){}, NULL,
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, (char*)data);
            
            currentSSID = doc["ssid"].as<String>();
            currentPassword = doc["password"].as<String>();
            saveCredentials();
            
            request->send(200, "application/json", "{\"status\":\"ok\"}");
            
            delay(500);
            ESP.restart();
    });
    
    // Fichiers statiques
    server.serveStatic("/", SPIFFS, "/");
    
    server.begin();
}

void WiFiManager::loadCredentials() {
    if (SPIFFS.exists("/wifi.json")) {
        File file = SPIFFS.open("/wifi.json", "r");
        if (file) {
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, file);
            currentSSID = doc["ssid"].as<String>();
            currentPassword = doc["password"].as<String>();
            file.close();
        }
    }
}

void WiFiManager::saveCredentials() {
    File file = SPIFFS.open("/wifi.json", "w");
    if (file) {
        DynamicJsonDocument doc(1024);
        doc["ssid"] = currentSSID;
        doc["password"] = currentPassword;
        serializeJson(doc, file);
        file.close();
    }
}

void WiFiManager::update() {
    if (!apMode && WiFi.status() != WL_CONNECTED) {
        connected = false;
        if (currentSSID.length() > 0) {
            startClient();
        } else {
            startAP();
        }
    }
}

void WiFiManager::sendData(const SensorData& data) {
    if (!connected) return;
    
    DynamicJsonDocument doc(2048);
    doc["timestamp"] = data.timestamp;
    doc["objectCount"] = data.objectCount;
    doc["confidence"] = data.confidence;
    
    JsonArray points = doc.createNestedArray("points");
    for (const auto& point : data.points) {
        JsonObject p = points.createNestedObject();
        p["x"] = point.x;
        p["y"] = point.y;
    }
    
    JsonArray labels = doc.createNestedArray("labels");
    for (const auto& label : data.labels) {
        labels.add(label);
    }
    
    String output;
    serializeJson(doc, output);
    
    // Envoyer à tous les clients connectés via WebSocket
    // TODO: Implémenter WebSocket
}

bool WiFiManager::isConnected() const {
    return connected;
}

String WiFiManager::getIPAddress() const {
    if (apMode) {
        return WiFi.softAPIP().toString();
    }
    return WiFi.localIP().toString();
}