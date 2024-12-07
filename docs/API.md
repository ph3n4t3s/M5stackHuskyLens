# Référence API

## Vision et Reconnaissance

### HuskyLensPlus
```cpp
class HuskyLensPlus {
public:
    bool begin();
    void update();
    void setMode(HuskyMode mode);
    SensorData getData();
    bool isConnected();
    void learn(int id);
    void forget();
    void saveModel();
};
```

### ImageProcessor
```cpp
class ImageProcessor {
public:
    bool begin();
    void processImage(SensorData& data);
    void applyFilter(const String& filterName);
    void addCustomFilter(const String& name, const float kernel[3][3]);
    void setEnhancement(const ImageEnhancement& enhancement);
    void enableNightMode(bool enable);
    std::vector<Point> detectEdges();
};
```

## Intelligence Artificielle

### MLSystem
```cpp
class MLSystem {
public:
    bool begin();
    void addModel(const String& name, const std::vector<float>& weights,
                 int inputSize, int outputSize);
    std::vector<float> predict(const String& modelName,
                             const std::vector<float>& input);
    bool train(const String& modelName,
              const std::vector<std::vector<float>>& features,
              const std::vector<std::vector<float>>& labels);
};
```

### GestureAnalyzer
```cpp
class GestureAnalyzer {
public:
    void addPattern(const String& name, const std::vector<Point>& pattern);
    void addPoint(const Point& point);
    String recognizeGesture();
    float getConfidence();
};
```

## Automatisation

### AutomationSystem
```cpp
class AutomationSystem {
public:
    bool begin();
    void update(const SensorData& data);
    void addRule(const Rule& rule);
    void enableRule(const String& name, bool enable);
    bool saveRules(const String& filename);
    bool loadRules(const String& filename);
};
```

### Rule Structure
```cpp
struct Rule {
    String name;
    String description;
    bool enabled;
    bool oneShot;
    unsigned long cooldown;
    std::vector<Condition> conditions;
    std::vector<Action> actions;
};
```

## Configuration et Logging

### ConfigManager
```cpp
class ConfigManager {
public:
    bool begin();
    void saveConfig(const Configuration& config);
    Configuration loadConfig();
    void resetToDefaults();
    void setConfigChangeCallback(void (*callback)(const Configuration&));
};
```

### DataLogger
```cpp
class DataLogger {
public:
    bool begin();
    void log(const SensorData& data);
    void logError(const String& error);
    void logDebug(const String& message);
    bool exportCSV(const String& filename);
    void clearLogs();
};
```

## Connectivité

### WiFiManager
```cpp
class WiFiManager {
public:
    bool begin(const char* ssid = nullptr, const char* password = nullptr);
    void startAP();
    void startClient();
    bool isConnected();
    String getIPAddress();
    void sendData(const SensorData& data);
};
```

## Structures de Données

### SensorData
```cpp
struct SensorData {
    std::vector<Point> points;
    std::vector<String> labels;
    int objectCount;
    float confidence;
    unsigned long timestamp;
};
```

### Configuration
```cpp
struct Configuration {
    bool autoLearn;
    int sensitivity;
    bool audioFeedback;
    int zoomLevel;
    bool nightMode;
    unsigned long recordingInterval;
};
```

### ImageEnhancement
```cpp
struct ImageEnhancement {
    float brightness;
    float contrast;
    float saturation;
    float sharpness;
    bool nightMode;
};
```

## Événements et Callbacks

### Configuration Change
```cpp
void onConfigChange(const Configuration& newConfig);
```

### Automation Triggers
```cpp
std::function<bool(const SensorData&)> objectDetected(const String& objectName);
std::function<bool(const SensorData&)> gestureDetected(const String& gestureName);
std::function<bool(const SensorData&)> positionInZone(const Point& center, float radius);
```

## Constantes

### Display Modes
```cpp
enum class DisplayMode {
    RAW_DATA,
    PROCESSED_INFO,
    GRAPHIC_INTERFACE,
    ANALYTICS_VIEW,
    SPLIT_SCREEN,
    DEBUGGING_VIEW
};
```

### HuskyLens Modes
```cpp
enum class HuskyMode {
    FACE_RECOGNITION,
    OBJECT_TRACKING,
    LINE_TRACKING,
    COLOR_RECOGNITION,
    TAG_RECOGNITION,
    GESTURE_RECOGNITION,
    DISTANCE_MEASUREMENT,
    MULTI_OBJECT_FUSION
};
```

## Exemples d'Utilisation

### Configuration basique
```cpp
void setup() {
    huskyLens.begin();
    imageProcessor.begin();
    mlSystem.begin();
    automationSystem.begin();
    
    // Charger la configuration
    config = configManager.loadConfig();
}
```

### Traitement d'image
```cpp
void processFrame(SensorData& data) {
    imageProcessor.setEnhancement({
        .brightness = 1.2f,
        .contrast = 1.1f,
        .nightMode = true
    });
    
    imageProcessor.processImage(data);
    
    std::vector<Point> edges = imageProcessor.detectEdges();
    // Utiliser les résultats...
}
```

### Automatisation
```cpp
void setupAutomation() {
    Rule rule;
    rule.name = "DetectObject";
    rule.conditions.push_back({
        TriggerType::OBJECT_DETECTED,
        {"person"}
    });
    rule.actions.push_back({
        ActionType::SEND_NOTIFICATION,
        {"Personne détectée!"}
    });
    
    automationSystem.addRule(rule);
}
```

### Machine Learning
```cpp
void setupML() {
    std::vector<float> weights = loadWeights("model.bin");
    mlSystem.addModel("objectDetector", weights, 64, 10);
    
    std::vector<float> features = MLSystem::extractFeatures(data);
    std::vector<float> predictions = mlSystem.predict("objectDetector", features);
}
```