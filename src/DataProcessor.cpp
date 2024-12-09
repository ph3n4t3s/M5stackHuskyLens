#include "DataProcessor.h"
#include <cmath>

DataProcessor::DataProcessor() : currentMode(HuskyMode::FACE_RECOGNITION) {
    // Initialisation sans reserve() car deque n'en a pas besoin
}

void DataProcessor::begin() {
    displayData = DisplayData();
}

void DataProcessor::process(const SensorData& data) {
    displayData = DisplayData();
    displayData.needsUpdate = true;
    displayData.points = data.points;
    
    switch (currentMode) {
        case HuskyMode::GESTURE_RECOGNITION:
            processGestureData(data);
            break;
        case HuskyMode::DISTANCE_MEASUREMENT:
            processDistanceData(data);
            break;
        case HuskyMode::MULTI_OBJECT_FUSION:
            processMultiObjectData(data);
            break;
        default:
            processStandardData(data);
    }
    
    updateHistory(data);
    analyzeData();
}

void DataProcessor::processGestureData(const SensorData& data) {
    static std::vector<Point> gesturePoints;
    static unsigned long lastGestureTime = 0;
    const unsigned long GESTURE_TIMEOUT = 1000; // 1 seconde
    
    // Réinitialisation si trop de temps s'est écoulé
    if (millis() - lastGestureTime > GESTURE_TIMEOUT) {
        gesturePoints.clear();
    }
    
    // Ajout des nouveaux points
    for (const Point& p : data.points) {
        gesturePoints.push_back(p);
        lastGestureTime = millis();
    }
    
    // Analyse du geste si assez de points collectés
    if (gesturePoints.size() >= 10) {
        int dx = gesturePoints.back().x - gesturePoints.front().x;
        int dy = gesturePoints.back().y - gesturePoints.front().y;
        
        String gesture;
        if (abs(dx) > abs(dy)) {
            gesture = dx > 0 ? "→ Droite" : "← Gauche";
        } else {
            gesture = dy > 0 ? "↓ Bas" : "↑ Haut";
        }
        
        displayData.labels.push_back(gesture);
        displayData.confidences.push_back(100.0f);
        gesturePoints.clear();
    }
}

void DataProcessor::processDistanceData(const SensorData& data) {
    const float CALIBRATION_FACTOR = 0.5f;
    
    for (size_t i = 0; i < data.points.size(); i++) {
        const Point& p = data.points[i];
        
        float screenDistance = sqrt(pow(p.x - Constants::Display::SCREEN_WIDTH/2.0f, 2) + 
                                 pow(p.y - Constants::Display::SCREEN_HEIGHT/2.0f, 2));
        
        float distance = screenDistance * CALIBRATION_FACTOR;
        
        displayData.labels.push_back(String(distance, 1) + " cm");
        displayData.confidences.push_back(data.confidence);
    }
}

void DataProcessor::processMultiObjectData(const SensorData& data) {
    static std::vector<Point> lastPositions;
    static unsigned long lastUpdateTime = millis();
    unsigned long currentTime = millis();
    float deltaTime = (currentTime - lastUpdateTime) / 1000.0f;
    
    for (size_t i = 0; i < data.points.size(); i++) {
        String info;
        if (i < lastPositions.size() && deltaTime > 0) {
            int dx = data.points[i].x - lastPositions[i].x;
            int dy = data.points[i].y - lastPositions[i].y;
            float speed = sqrt(dx*dx + dy*dy) / deltaTime;
            
            info = String(speed, 1) + " px/s ";
            if (abs(dx) > abs(dy)) {
                info += dx > 0 ? "→" : "←";
            } else {
                info += dy > 0 ? "↓" : "↑";
            }
        } else {
            info = "Nouvel objet";
        }
        
        displayData.labels.push_back(info);
        displayData.confidences.push_back(data.confidence);
    }
    
    lastPositions = data.points;
    lastUpdateTime = currentTime;
}

void DataProcessor::processStandardData(const SensorData& data) {
    for (size_t i = 0; i < data.points.size(); i++) {
        displayData.labels.push_back("Obj " + String(i + 1));
        displayData.confidences.push_back(data.confidence);
    }
}

void DataProcessor::updateHistory(const SensorData& data) {
    if (history.size() >= Constants::History::MAX_HISTORY) {
        history.pop_front();
    }
    
    history.push_back(HistoricalData(
        data.timestamp,
        static_cast<int>(currentMode),
        data.objectCount,
        data.confidence
    ));
}

void DataProcessor::analyzeData() {
    if (history.empty()) return;
    
    std::vector<float> trends = calculateTrends();
    float avgTrend = 0.0f;
    
    if (!trends.empty()) {
        for (float trend : trends) {
            avgTrend += trend;
        }
        avgTrend /= trends.size();
    }
    
    String status = "Objets: " + String(displayData.points.size());
    status += " | Tend.: ";
    if (avgTrend > 0.1f) status += "↑";
    else if (avgTrend < -0.1f) status += "↓";
    else status += "→";
    
    displayData.statusMessage = status;
}

std::vector<float> DataProcessor::calculateTrends() const {
    std::vector<float> trends;
    
    if (history.size() < 2) return trends;
    
    for (size_t i = 1; i < history.size(); i++) {
        float trend = history[i].detections - history[i-1].detections;
        trends.push_back(trend);
    }
    
    return trends;
}

void DataProcessor::setMode(HuskyMode mode) {
    if (currentMode != mode) {
        currentMode = mode;
        history.clear();
        displayData = DisplayData();
    }
}

DisplayData DataProcessor::getDisplayData() const {
    return displayData;
}