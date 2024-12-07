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
            // QR Code n'est pas encore supporté, utiliser la reconnaissance d'objets par défaut
            huskyLens.writeAlgorithm(ALGORITHM_OBJECT_RECOGNITION);
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
    huskyLens.writeLearn(id);
}

void HuskyLensPlus::forget() {
    huskyLens.writeForget();
}

void HuskyLensPlus::saveModel() {
    // Cette fonctionnalité n'est pas disponible dans la version actuelle
    // TODO: Implémenter la sauvegarde sur carte SD
}

float HuskyLensPlus::calculateDistance(int width, int height) const {
    const float REFERENCE_SIZE = 100.0f;
    const float REFERENCE_DISTANCE = 50.0f;
    float size = sqrt(width * width + height * height);
    return (REFERENCE_SIZE * REFERENCE_DISTANCE) / size;
}

void HuskyLensPlus::handleGestures() {
    static std::vector<Point> gesturePoints;
    static unsigned long lastGestureTime = 0;
    const unsigned long GESTURE_TIMEOUT = 1000;
    
    if (millis() - lastGestureTime > GESTURE_TIMEOUT) {
        gesturePoints.clear();
    }
    
    // Ajouter les nouveaux points au geste
    for (const Point& p : currentData.points) {
        gesturePoints.push_back(p);
        lastGestureTime = millis();
    }
    
    // Analyser le geste si assez de points
    if (gesturePoints.size() >= 10) {
        int dx = gesturePoints.back().x - gesturePoints.front().x;
        int dy = gesturePoints.back().y - gesturePoints.front().y;
        
        String gesture;
        if (abs(dx) > abs(dy)) {
            gesture = dx > 0 ? "Droite" : "Gauche";
        } else {
            gesture = dy > 0 ? "Bas" : "Haut";
        }
        
        // Ajout de la reconnaissance de gestes circulaires
        float totalAngle = 0;
        for (size_t i = 2; i < gesturePoints.size(); i++) {
            Point p1 = gesturePoints[i-2];
            Point p2 = gesturePoints[i-1];
            Point p3 = gesturePoints[i];
            
            float angle = atan2(p3.y - p2.y, p3.x - p2.x) - atan2(p2.y - p1.y, p2.x - p1.x);
            if (angle > M_PI) angle -= 2*M_PI;
            if (angle < -M_PI) angle += 2*M_PI;
            totalAngle += angle;
        }
        
        if (abs(totalAngle) > M_PI) {
            gesture = totalAngle > 0 ? "Cercle Anti-H" : "Cercle H";
        }
        
        currentData.labels.clear();
        currentData.labels.push_back(gesture);
        gesturePoints.clear();
    }
}

void HuskyLensPlus::handleDistance() {
    for (size_t i = 0; i < currentData.points.size(); i++) {
        if (i < currentData.objectCount) {
            HUSKYLENSResult result = huskyLens.get(i);
            float distance = calculateDistance(result.width, result.height);
            String info = String(distance, 1) + " cm";
            
            if (currentData.labels.size() <= i) {
                currentData.labels.push_back(info);
            } else {
                currentData.labels[i] = info;
            }
        }
    }
}

void HuskyLensPlus::handleMultiObject() {
    static std::vector<Point> lastPositions;
    static unsigned long lastUpdateTime = millis();
    const float deltaTime = (millis() - lastUpdateTime) / 1000.0f;
    
    for (size_t i = 0; i < currentData.points.size(); i++) {
        String info;
        if (i < lastPositions.size() && deltaTime > 0) {
            float dx = currentData.points[i].x - lastPositions[i].x;
            float dy = currentData.points[i].y - lastPositions[i].y;
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
        
        if (currentData.labels.size() <= i) {
            currentData.labels.push_back(info);
        } else {
            currentData.labels[i] = info;
        }
    }
    
    lastPositions = currentData.points;
    lastUpdateTime = millis();
}
