#include "HuskyLensPlus.h"
#include "SD.h"
#include "FS.h"
#include "SPIFFS.h"

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
            case HuskyMode::QR_CODE:
                handleQRCode();
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
    protocolAlgorithm algo;
    switch (mode) {
        case HuskyMode::FACE_RECOGNITION:
            algo = ALGORITHM_FACE_RECOGNITION;
            break;
        case HuskyMode::OBJECT_TRACKING:
            algo = ALGORITHM_OBJECT_TRACKING;
            break;
        case HuskyMode::LINE_TRACKING:
            algo = ALGORITHM_LINE_TRACKING;
            break;
        case HuskyMode::COLOR_RECOGNITION:
            algo = ALGORITHM_COLOR_RECOGNITION;
            break;
        case HuskyMode::TAG_RECOGNITION:
            algo = ALGORITHM_TAG_RECOGNITION;
            break;
        case HuskyMode::OBJECT_CLASSIFICATION:
            algo = ALGORITHM_OBJECT_CLASSIFICATION;
            break;
        case HuskyMode::QR_CODE:
        case HuskyMode::CUSTOM_CARDS:
            algo = ALGORITHM_TAG_RECOGNITION;
            break;
        default:
            algo = ALGORITHM_OBJECT_TRACKING;
            break;
    }
    huskyLens.writeAlgorithm(algo);
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
    // Envoi de la commande de sauvegarde via I2C
    Wire.beginTransmission(0x32);
    Wire.write(0x39);  // Commande de sauvegarde
    if (Wire.endTransmission() == 0) {
        Serial.println("Model saved to internal memory");
        delay(1000);  // Attendre que la sauvegarde soit terminée
    } else {
        Serial.println("Error: Could not save model");
    }
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

void HuskyLensPlus::handleQRCode() {
    if (huskyLens.available()) {
        HUSKYLENSResult result = huskyLens.read();
        if (result.command == COMMAND_RETURN_OK && result.ID > 0) {  // ID > 0 indique une détection valide
            String qrData = String(result.ID);  // Pour l'instant, on utilise l'ID comme donnée
            
            // Ajouter des métadonnées
            if (currentData.points.size() > 0) {
                Point center = currentData.points.back();
                String pos = String(center.x) + "," + String(center.y);
                currentData.labels.push_back("Tag ID: " + qrData);
                currentData.labels.push_back("Pos: " + pos);
                
                // Sauvegarder l'historique si la carte SD est disponible
                if (SD.begin()) {
                    manageQRLogFile();
                    File logFile = SD.open(Constants::QR_LOG_FILE, FILE_APPEND);
                    if (logFile) {
                        String logEntry = String(millis()) + ",Tag:" + qrData + ",Pos:" + pos + "\n";
                        logFile.print(logEntry);
                        logFile.close();
                    }
                }
            }
        }
    }
}

void HuskyLensPlus::manageQRLogFile() const {
    if (!SD.exists(Constants::QR_LOG_FILE)) return;
    
    File logFile = SD.open(Constants::QR_LOG_FILE, FILE_READ);
    if (!logFile) return;
    
    // Compter les lignes
    int lineCount = 0;
    while (logFile.available()) {
        char c = logFile.read();
        if (c == '\n') lineCount++;
    }
    logFile.close();
    
    // Si le nombre de lignes dépasse la limite
    if (lineCount > Constants::MAX_QR_LOG_ENTRIES) {
        // Créer un fichier temporaire
        File tempFile = SD.open("/temp_qr.txt", FILE_WRITE);
        if (!tempFile) return;
        
        logFile = SD.open(Constants::QR_LOG_FILE, FILE_READ);
        if (!logFile) {
            tempFile.close();
            return;
        }
        
        // Ignorer les premières lignes
        int linesToSkip = lineCount - Constants::MAX_QR_LOG_ENTRIES;
        for (int i = 0; i < linesToSkip; i++) {
            while (logFile.available() && logFile.read() != '\n');
        }
        
        // Copier les lignes restantes
        while (logFile.available()) {
            tempFile.write(logFile.read());
        }
        
        logFile.close();
        tempFile.close();
        
        // Remplacer l'ancien fichier
        SD.remove(Constants::QR_LOG_FILE);
        SD.rename("/temp_qr.txt", Constants::QR_LOG_FILE);
    }
}