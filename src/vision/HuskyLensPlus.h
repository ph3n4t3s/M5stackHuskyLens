#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "HUSKYLENS.h"
#include "Config.h"
#include <vector>

class HuskyLensPlus {
public:
    HuskyLensPlus();
    bool begin();
    void update();
    void setMode(HuskyMode mode);
    SensorData getData() const;
    bool isConnected() const;
    void learn(int id);
    void forget();
    void saveModel();
    
private:
    HUSKYLENS huskyLens;
    HuskyMode currentMode;
    SensorData currentData;
    bool connected;
    
    void configureMode(HuskyMode mode);
    void processData();
    void handleGestures();
    void handleDistance();
    void handleMultiObject();
    void handleQRCode();
    float calculateDistance(int width, int height) const;
    void manageQRLogFile() const;
};
