# Implémentation M5Stack - Protocole de Communication

## 1. Structure du code M5Stack

### 1.1 Organisation des fichiers
```
src/
├── main.cpp
├── config.h
├── communication/
│   ├── MQTTHandler.h
│   ├── MQTTHandler.cpp
│   ├── USBSerial.h
│   └── USBSerial.cpp
├── hardware/
│   ├── DisplayManager.h
│   ├── DisplayManager.cpp
│   ├── HuskyLensDriver.h
│   └── HuskyLensDriver.cpp
├── mission/
│   ├── MissionManager.h
│   ├── MissionManager.cpp
│   ├── Validator.h
│   └── Validator.cpp
└── utils/
    ├── JsonParser.h
    ├── JsonParser.cpp
    ├── Logger.h
    └── Logger.cpp
```

### 1.2 main.cpp
```cpp
#include <M5CoreS3.h>
#include "communication/MQTTHandler.h"
#include "hardware/DisplayManager.h"
#include "hardware/HuskyLensDriver.h"
#include "mission/MissionManager.h"

MQTTHandler mqtt;
DisplayManager display;
HuskyLensDriver huskylens;
MissionManager missions;

void setup() {
    // Initialisation du matériel
    M5.begin();
    display.init();
    huskylens.init();
    
    // Configuration MQTT
    mqtt.connect();
    mqtt.subscribe("m5stack/command/#");
    
    // Affichage initial
    display.showStartScreen();
}

void loop() {
    // Gestion des messages MQTT
    mqtt.handleMessages();
    
    // Mise à jour HuskyLens
    huskylens.update();
    
    // Gestion des missions
    missions.update();
    
    // Mise à jour affichage
    display.update();
    
    // Gestion des boutons
    M5.update();
}
```

## 2. Gestion MQTT

### 2.1 MQTTHandler.h
```cpp
class MQTTHandler {
private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    QueueHandle_t commandQueue;
    
    static void callback(char* topic, byte* payload, unsigned int length);
    void processCommand(const char* payload);
    
public:
    MQTTHandler();
    bool connect();
    void disconnect();
    void publish(const char* topic, const char* payload);
    void subscribe(const char* topic);
    void handleMessages();
};
```

### 2.2 MQTTHandler.cpp
```cpp
void MQTTHandler::processCommand(const char* payload) {
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        Logger::error("JSON parsing failed");
        return;
    }
    
    const char* command = doc["command"];
    JsonObject params = doc["parameters"];
    
    // Gestion des commandes
    if (strcmp(command, "display_text") == 0) {
        display.showText(
            params["text"],
            params["x"] | 0,
            params["y"] | 0,
            params["size"] | 2
        );
    }
    else if (strcmp(command, "set_mode") == 0) {
        huskylens.setMode(params["mode"]);
    }
    // ... autres commandes
}
```

## 3. Gestion HuskyLens

### 3.1 HuskyLensDriver.h
```cpp
class HuskyLensDriver {
private:
    HUSKYLENS huskylens;
    String currentMode;
    bool isConnected;
    
    void processFrame();
    void publishResults();
    
public:
    HuskyLensDriver();
    bool init();
    void setMode(const char* mode);
    void update();
    bool isLearning();
    void startLearning(int id);
    void stopLearning();
};
```

### 3.2 HuskyLensDriver.cpp
```cpp
void HuskyLensDriver::processFrame() {
    if (!huskylens.request()) {
        Logger::error("HuskyLens request failed");
        return;
    }
    
    StaticJsonDocument<1024> doc;
    JsonArray objects = doc.createNestedArray("objects");
    
    while (huskylens.available()) {
        HUSKYLENSResult result = huskylens.read();
        JsonObject obj = objects.createNestedObject();
        
        obj["id"] = result.ID;
        obj["x"] = result.xCenter;
        obj["y"] = result.yCenter;
        obj["width"] = result.width;
        obj["height"] = result.height;
    }
    
    char buffer[1024];
    serializeJson(doc, buffer);
    mqtt.publish("huskylens/data", buffer);
}
```

## 4. Gestion des missions

### 4.1 MissionManager.h
```cpp
class MissionManager {
private:
    int currentMission;
    bool missionActive;
    Validator validator;
    
    void startMission(int id);
    void endMission();
    bool validateMission(JsonObject result);
    
public:
    MissionManager();
    void update();
    void handleMissionCommand(const char* command, JsonObject params);
    bool isMissionActive();
};
```

### 4.2 MissionManager.cpp
```cpp
void MissionManager::handleMissionCommand(const char* command, JsonObject params) {
    if (strcmp(command, "start_mission") == 0) {
        int missionId = params["mission_id"];
        startMission(missionId);
    }
    else if (strcmp(command, "validate_mission") == 0) {
        if (validateMission(params["submission"])) {
            mqtt.publish("mission/complete", "success");
            endMission();
        } else {
            mqtt.publish("mission/complete", "failure");
        }
    }
}
```

## 5. Validation des résultats

### 5.1 Validator.h
```cpp
class Validator {
private:
    struct MissionCriteria {
        int minObjects;
        int maxObjects;
        float minConfidence;
        String requiredMode;
    };
    
    std::map<int, MissionCriteria> criteria;
    
public:
    Validator();
    void loadCriteria();
    bool validateSubmission(int missionId, JsonObject submission);
};
```

### 5.2 Validator.cpp
```cpp
bool Validator::validateSubmission(int missionId, JsonObject submission) {
    if (!criteria.count(missionId)) {
        Logger::error("Invalid mission ID");
        return false;
    }
    
    MissionCriteria& mc = criteria[missionId];
    
    // Validation du mode
    if (submission["mode"] != mc.requiredMode) {
        return false;
    }
    
    // Validation des objets détectés
    JsonArray objects = submission["results"]["objects"];
    int objectCount = objects.size();
    
    if (objectCount < mc.minObjects || objectCount > mc.maxObjects) {
        return false;
    }
    
    // Validation de la confiance
    for (JsonObject obj : objects) {
        if (obj["confidence"] < mc.minConfidence) {
            return false;
        }
    }
    
    return true;
}
```

## 6. Gestion des erreurs

### 6.1 ErrorHandler.h
```cpp
class ErrorHandler {
private:
    struct Error {
        String code;
        String message;
        bool critical;
    };
    
    QueueHandle_t errorQueue;
    
public:
    ErrorHandler();
    void reportError(const char* code, const char* message, bool critical = false);
    void handleErrors();
};
```

### 6.2 ErrorHandler.cpp
```cpp
void ErrorHandler::reportError(const char* code, const char* message, bool critical) {
    StaticJsonDocument<256> doc;
    
    doc["code"] = code;
    doc["message"] = message;
    doc["timestamp"] = getTimestamp();
    doc["critical"] = critical;
    
    char buffer[256];
    serializeJson(doc, buffer);
    
    mqtt.publish("system/error", buffer);
    
    if (critical) {
        display.showError(message);
    }
}
```

## 7. Configuration

### 7.1 config.h
```cpp
#define WIFI_SSID "M5Stack_Network"
#define WIFI_PASSWORD "m5stack_secret"

#define MQTT_BROKER "192.168.1.100"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "m5stack_main"

#define HUSKYLENS_PROTOCOL PROTOCOL_I2C
#define HUSKYLENS_ADDRESS 0x32

#define DISPLAY_BRIGHTNESS 80
#define DISPLAY_ROTATION 1

#define MAX_QUEUE_SIZE 50
#define COMMAND_TIMEOUT 5000
```

## 8. Optimisations

### 8.1 Mémoire
```cpp
// Utilisation de la mémoire PSRAM pour les buffers
class BufferManager {
private:
    uint8_t* displayBuffer;
    size_t bufferSize;
    
public:
    BufferManager(size_t size) {
        if (psramFound()) {
            displayBuffer = (uint8_t*)ps_malloc(size);
        } else {
            displayBuffer = (uint8_t*)malloc(size);
        }
    }
};
```

### 8.2 Performance
```cpp
// File d'attente des commandes optimisée
template<typename T>
class CommandQueue {
private:
    static const size_t QUEUE_SIZE = 32;
    T buffer[QUEUE_SIZE];
    size_t head;
    size_t tail;
    
public:
    bool push(const T& item) {
        if (isFull()) return false;
        buffer[tail] = item;
        tail = (tail + 1) % QUEUE_SIZE;
        return true;
    }
};
```

## 9. Tests unitaires

### 9.1 test_mqtt.cpp
```cpp
void test_mqtt_connection() {
    MQTTHandler mqtt;
    assert(mqtt.connect());
    
    // Test publication
    mqtt.publish("test/topic", "message");
    delay(100);
    assert(messageReceived);
    
    // Test souscription
    mqtt.subscribe("test/command");
    delay(100);
    assert(subscriptionActive);
}
```

### 9.2 test_huskylens.cpp
```cpp
void test_huskylens_modes() {
    HuskyLensDriver husky;
    assert(husky.init());
    
    // Test modes
    husky.setMode("FACE_RECOGNITION");
    delay(100);
    assert(husky.getCurrentMode() == "FACE_RECOGNITION");
    
    // Test capture
    assert(husky.request());
    delay(100);
    assert(husky.available());
}
```