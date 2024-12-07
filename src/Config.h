#pragma once

#include <Arduino.h>
#include <vector>

// Structures de base
struct Configuration {
    bool autoLearn;
    int sensitivity;
    bool audioFeedback;
    int zoomLevel;
    bool nightMode;
    unsigned long recordingInterval;
    
    Configuration() :
        autoLearn(false),
        sensitivity(3),
        audioFeedback(true),
        zoomLevel(1),
        nightMode(false),
        recordingInterval(1000)
    {}
};

struct Point {
    int x;
    int y;
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

struct DetectionZone {
    int x, y, width, height;
    bool active;
    String label;
    
    DetectionZone(int _x = 0, int _y = 0, int _w = 0, int _h = 0, const String& _label = "") :
        x(_x), y(_y), width(_w), height(_h), active(true), label(_label)
    {}
};

struct HistoricalData {
    unsigned long timestamp;
    int mode;
    int detections;
    float confidence;
    
    // Constructeur par défaut
    HistoricalData() : 
        timestamp(0), 
        mode(0), 
        detections(0), 
        confidence(0.0f) 
    {}

    // Constructeur avec paramètres
    HistoricalData(unsigned long ts, int m, int d, float c) :
        timestamp(ts), 
        mode(m), 
        detections(d), 
        confidence(c) 
    {}
};

struct SensorData {
    std::vector<Point> points;
    std::vector<String> labels;
    int objectCount;
    float confidence;
    unsigned long timestamp;
    
    SensorData() : 
        objectCount(0), 
        confidence(0.0f), 
        timestamp(0) 
    {}
};

struct DisplayData {
    std::vector<Point> points;
    std::vector<String> labels;
    std::vector<float> confidences;
    String statusMessage;
    bool needsUpdate;
    
    DisplayData() : needsUpdate(false) {}
};

// Énumérations
enum class HuskyMode {
    FACE_RECOGNITION,
    OBJECT_TRACKING,
    LINE_TRACKING,
    COLOR_RECOGNITION,
    TAG_RECOGNITION,
    OBJECT_CLASSIFICATION,
    QR_CODE,
    CUSTOM_CARDS,
    MOTION_TRACKING,
    GESTURE_RECOGNITION,
    DISTANCE_MEASUREMENT,
    MULTI_OBJECT_FUSION
};

enum class DisplayMode {
    RAW_DATA,
    PROCESSED_INFO,
    GRAPHIC_INTERFACE,
    ANALYTICS_VIEW,
    SPLIT_SCREEN,
    DEBUGGING_VIEW
};

// Constantes globales
namespace Constants {
    const int SCREEN_WIDTH = 320;
    const int SCREEN_HEIGHT = 240;
    const int MAX_HISTORY = 100;
    const int MAX_OBJECTS = 25;
    const int MENU_ITEMS = 6;
    const unsigned long LEARN_INTERVAL = 5000;
    
    // Constantes d'affichage
    const int STATUS_BAR_HEIGHT = 20;
    const int MENU_ITEM_HEIGHT = 30;
    const int TEXT_SIZE_NORMAL = 2;
    const int TEXT_SIZE_SMALL = 1;
    
    // Couleurs (utilisant les définitions M5Stack)
    const uint16_t COLOR_BACKGROUND = 0x0000;   // Noir
    const uint16_t COLOR_TEXT = 0xFFFF;         // Blanc
    const uint16_t COLOR_HIGHLIGHT = 0x051D;    // Bleu
    const uint16_t COLOR_WARNING = 0xFFE0;      // Jaune
    const uint16_t COLOR_ERROR = 0xF800;        // Rouge
    const uint16_t COLOR_SUCCESS = 0x07E0;      // Vert
}