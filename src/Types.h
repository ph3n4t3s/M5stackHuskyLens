#pragma once

#include <Arduino.h>
#include "Constants.h"
#include <vector>

// Étapes de démarrage
enum class BootStage {
    INIT,
    HARDWARE_CHECK,
    WIFI_SETUP,
    HUSKYLENS_INIT,
    COMPLETE,
    ERROR
};

// Structure de point 2D
struct Point {
    int x;
    int y;

    Point() : x(0), y(0) {}
    Point(int _x, int _y) : x(_x), y(_y) {}
    
    float distanceTo(const Point& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return sqrt(dx * dx + dy * dy);
    }
};

// Structure de rectangle
struct Rect {
    int x, y, width, height;
    
    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(int _x, int _y, int w, int h) : x(_x), y(_y), width(w), height(h) {}
    
    bool contains(const Point& p) const {
        return p.x >= x && p.x <= x + width && p.y >= y && p.y <= y + height;
    }
    
    Point center() const {
        return Point(x + width/2, y + height/2);
    }
};

// Message de démarrage
struct BootMessage {
    String message;
    uint32_t color;
    unsigned long timestamp;
    
    BootMessage(const String& msg, uint32_t col) :
        message(msg), color(col), timestamp(millis()) {}
};

// État du système
enum class SystemState {
    BOOTING,
    SETUP_REQUIRED,
    CONNECTING_WIFI,
    CONNECTING_HUSKYLENS,
    READY,
    ERROR,
    SETTINGS
};

// Mode d'affichage
enum class DisplayMode {
    BOOT_SEQUENCE,       // Séquence de démarrage
    NORMAL_OPERATION,    // Fonctionnement normal
    MAIN,               // Interface principale
    MENU,               // Menu principal
    SETTINGS,           // Paramètres
    QR_CODE,            // Affichage QR code
    ERROR               // Message d'erreur
};

// Mode de reconnaissance HuskyLens
enum class HuskyMode {
    FACE_RECOGNITION,     // Reconnaissance faciale
    GESTURE_RECOGNITION,  // Reconnaissance de gestes
    DISTANCE_MEASUREMENT, // Mesure de distance
    MULTI_OBJECT_FUSION,  // Fusion multi-objets
    QR_CODE,             // Lecture de QR code
    UNKNOWN
};

// Données de détection
struct DetectionData {
    std::vector<Point> points;
    std::vector<String> labels;
    std::vector<float> confidences;
    std::vector<Rect> boundingBoxes;
    HuskyMode mode;
    unsigned long timestamp;

    DetectionData() : 
        mode(HuskyMode::UNKNOWN),
        timestamp(millis()) {}
};

// Configuration de l'affichage
struct DisplayConfig {
    uint8_t brightness;         // 0-255
    bool nightMode;            // Mode nuit
    int rotation;             // Rotation de l'écran
    uint32_t backgroundColor;  // Couleur de fond
    uint32_t textColor;       // Couleur du texte
    
    DisplayConfig() :
        brightness(Constants::Display::DEFAULT_BRIGHTNESS),
        nightMode(false),
        rotation(Constants::Display::DEFAULT_ROTATION),
        backgroundColor(Constants::Colors::BACKGROUND),
        textColor(Constants::Colors::TEXT) {}
};

// Paramètres WiFi
struct WiFiConfig {
    char ssid[32];
    char password[64];
    bool autoConnect;
    unsigned long lastConnection;
    
    WiFiConfig() : autoConnect(true), lastConnection(0) {
        ssid[0] = 0;
        password[0] = 0;
    }
};

// Configuration du système
struct SystemConfig {
    String deviceName;
    uint8_t volume;
    bool debugMode;
    bool firstBoot;
    DisplayConfig display;
    WiFiConfig wifi;
    
    SystemConfig() :
        deviceName("M5HuskyLens"),
        volume(128),
        debugMode(false),
        firstBoot(true) {}
};

// État du matériel
struct HardwareStatus {
    bool m5StackOK;
    bool huskyLensOK;
    bool wifiOK;
    bool sdCardOK;
    float batteryLevel;
    float temperature;
    
    HardwareStatus() :
        m5StackOK(false),
        huskyLensOK(false),
        wifiOK(false),
        sdCardOK(false),
        batteryLevel(0.0f),
        temperature(0.0f) {}
};

// Message d'erreur
struct ErrorInfo {
    String message;
    String details;
    bool recoverable;
    unsigned long timestamp;
    
    ErrorInfo() : recoverable(true), timestamp(0) {}
    ErrorInfo(const String& msg, bool rec = true) :
        message(msg),
        recoverable(rec),
        timestamp(millis()) {}
};

// Données historiques
struct HistoricalData {
    unsigned long timestamp;
    int mode;
    int detections;
    float confidence;

    HistoricalData(unsigned long t, int m, int d, float c) :
        timestamp(t),
        mode(m),
        detections(d),
        confidence(c) {}
};

// Zone de détection
struct DetectionZone {
    int x;
    int y;
    int width;
    int height;
    String label;
    bool active;

    DetectionZone() :
        x(0), y(0), width(0), height(0), active(false) {}

    DetectionZone(int _x, int _y, int w, int h, const String& l, bool a = true) :
        x(_x), y(_y), width(w), height(h), label(l), active(a) {}
};