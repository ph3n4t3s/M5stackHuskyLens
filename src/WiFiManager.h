#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include "api/WebServer.h"
#include <ArduinoJson.h>
#include "Config.h"

class WiFiManager {
public:
    WiFiManager();
    bool begin(const char* ssid = nullptr, const char* password = nullptr);
    void update();
    void startAP();
    void startClient();
    void stopAP();
    void handleClient();
    bool isConnected() const;
    String getIPAddress() const;
    void sendData(const SensorData& data);
    
private:
    AsyncWebServer server;
    bool apMode;
    bool connected;
    String currentSSID;
    String currentPassword;
    
    void setupWebServer();
    void handleRoot();
    void handleConfig();
    void handleData();
    void handleNotFound();
    void loadCredentials();
    void saveCredentials();
    
    static String getContentType(const String& filename);
    static void sendCORS();
};