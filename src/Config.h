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
#include "HUSKYLENS.h"

enum class HuskyMode {
    FACE_RECOGNITION = 0,
    OBJECT_TRACKING = 1,
    LINE_TRACKING = 2,
    COLOR_RECOGNITION = 3,
    TAG_RECOGNITION = 4,
    OBJECT_CLASSIFICATION = 5,
    QR_CODE = 6,
    CUSTOM_CARDS = 7,
    MOTION_TRACKING = 8,
    GESTURE_RECOGNITION = 9,
    DISTANCE_MEASUREMENT = 10,
    MULTI_OBJECT_FUSION = 11
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

    // QR Code constants
    const int MAX_QR_DATA_LENGTH = 256;
    const int MAX_QR_LOG_ENTRIES = 1000;
    const int QR_MIN_CONFIDENCE = 75;
    const String QR_LOG_FILE = "/qr_log.txt";
    
    // Model management constants
    const int MAX_SAVED_MODELS = 20;
    const String MODELS_DIRECTORY = "/models";
    const String MODEL_FILE_EXTENSION = ".model";
    const int MIN_MODEL_CONFIDENCE = 70;
    
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