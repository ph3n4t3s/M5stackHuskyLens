#include "HuskyLensPlus.h"

HuskyLensPlus::HuskyLensPlus() : 
    currentMode(HuskyMode::FACE_RECOGNITION),
    connected(false) {}

bool HuskyLensPlus::begin() {
    Wire.begin(2, 1);  // SDA=2, SCL=1 for M5Stack Core S3
    connected = huskyLens.begin(Wire);
    if (connected) {
        configureMode(currentMode);
    }
    return connected;
}

void HuskyLensPlus::update() {
    if (!connected || !huskyLens.request()) {
        connected = false;
        return;
    }
    
    currentData = SensorData();
    currentData.timestamp = millis();
    
    processData();
}

void HuskyLensPlus::processData() {
    while (huskyLens.available()) {
        HUSKYLENSResult result = huskyLens.read();
        
        Point p = {result.xCenter, result.yCenter};
        currentData.points.push_back(p);
        currentData.objectCount++;
        currentData.confidence = result.ID;  // Using ID as confidence proxy
        
        switch (currentMode) {
            case HuskyMode::GESTURE_RECOGNITION:
                handleGestures();
                break;
            case HuskyMode::DISTANCE_MEASUREMENT:
                handleDistance();
                break;
            case HuskyMode::MULTI_OBJECT_FUSION:
                handleMultiObject();
                break;
        }
    }
}

void HuskyLensPlus::setMode(HuskyMode mode) {
    if (mode != currentMode) {
        currentMode = mode;
        configureMode(mode);
    }
}

void HuskyLensPlus::configureMode(HuskyMode mode) {
    switch (mode) {
        case HuskyMode::FACE_RECOGNITION:
            huskyLens.writeAlgorithm(ALGORITHM_FACE_RECOGNITION);
            break;
        case HuskyMode::OBJECT_TRACKING:
            huskyLens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING);
            break;
        case HuskyMode::LINE_TRACKING:
            huskyLens.writeAlgorithm(ALGORITHM_LINE_TRACKING);
            break;
        case HuskyMode::COLOR_RECOGNITION:
            huskyLens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION);
            break;
        case HuskyMode::TAG_RECOGNITION:
            huskyLens.writeAlgorithm(ALGORITHM_TAG_RECOGNITION);
            break;
        case HuskyMode::OBJECT_CLASSIFICATION:
            huskyLens.writeAlgorithm(ALGORITHM_OBJECT_CLASSIFICATION);
            break;
        case HuskyMode::QR_CODE:
            huskyLens.writeAlgorithm(ALGORITHM_QR_CODE_RECOGNITION);
            break;
        default:
            huskyLens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING);
    }
}

SensorData HuskyLensPlus::getData() const {
    return currentData;
}

bool HuskyLensPlus::isConnected() const {
    return connected;
}

void HuskyLensPlus::learn(int id) {
    huskyLens.learn(id);
}

void HuskyLensPlus::forget() {
    huskyLens.forget();
}

void HuskyLensPlus::saveModel() {
    huskyLens.saveModelToTFCard();
}

float HuskyLensPlus::calculateDistance(int width, int height) const {
    const float REFERENCE_SIZE = 100.0f;
    const float REFERENCE_DISTANCE = 50.0f;
    float size = sqrt(width * width + height * height);
    return (REFERENCE_SIZE * REFERENCE_DISTANCE) / size;
}
