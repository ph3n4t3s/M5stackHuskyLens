#include "DataLogger.h"
#include <time.h>

DataLogger::DataLogger() : initialized(false), logLevel(1) {}

bool DataLogger::begin() {
    if (!SD.begin()) {
        return false;
    }
    
    initialized = checkAndCreateDirectory();
    return initialized;
}

void DataLogger::log(const SensorData& data) {
    if (!initialized || logLevel < 1) return;
    
    String message = "Objects: " + String(data.objectCount);
    message += ", Confidence: " + String(data.confidence);
    message += ", Points: " + String(data.points.size());
    
    if (!data.labels.empty()) {
        message += ", Labels: ";
        for (const auto& label : data.labels) {
            message += label + ",";
        }
    }
    
    writeToFile(formatLogMessage(message, "INFO"));
}

void DataLogger::logError(const String& error) {
    if (!initialized) return;
    
    String message = formatLogMessage(error, "ERROR");
    writeToFile(message);
    
    if (logBuffer.size() >= MAX_BUFFER_SIZE) {
        logBuffer.pop_front();
    }
    logBuffer.push_back(message);
}

void DataLogger::logDebug(const String& message) {
    if (!initialized || logLevel < 2) return;
    
    String logMessage = formatLogMessage(message, "DEBUG");
    writeToFile(logMessage);
    
    if (logBuffer.size() >= MAX_BUFFER_SIZE) {
        logBuffer.pop_front();
    }
    logBuffer.push_back(logMessage);
}

void DataLogger::setLogLevel(int level) {
    logLevel = level;
}

std::vector<String> DataLogger::getLastLogs(int count) {
    std::vector<String> result;
    int start = std::max(0, static_cast<int>(logBuffer.size()) - count);
    
    for (int i = start; i < logBuffer.size(); i++) {
        result.push_back(logBuffer[i]);
    }
    
    return result;
}

void DataLogger::clearLogs() {
    if (!initialized) return;
    
    File root = SD.open(LOG_DIRECTORY);
    File file = root.openNextFile();
    
    while (file) {
        if (!file.isDirectory()) {
            SD.remove(String(LOG_DIRECTORY) + "/" + String(file.name()));
        }
        file = root.openNextFile();
    }
    
    logBuffer.clear();
}

bool DataLogger::exportCSV(const String& filename) {
    if (!initialized) return false;
    
    File file = SD.open(LOG_DIRECTORY + "/" + filename, FILE_WRITE);
    if (!file) return false;
    
    file.println("Timestamp,Level,Message");
    for (const auto& log : logBuffer) {
        file.println(log);
    }
    
    file.close();
    return true;
}

void DataLogger::rotateLogFiles() {
    if (!initialized) return;
    
    const int MAX_LOG_FILES = 5;
    String baseFilename = LOG_DIRECTORY + "/log_";
    
    // Supprimer le plus ancien fichier
    SD.remove(baseFilename + String(MAX_LOG_FILES) + ".txt");
    
    // Rotation des fichiers
    for (int i = MAX_LOG_FILES - 1; i >= 0; i--) {
        String oldName = baseFilename + String(i) + ".txt";
        String newName = baseFilename + String(i + 1) + ".txt";
        if (SD.exists(oldName)) {
            SD.rename(oldName, newName);
        }
    }
}

String DataLogger::formatLogMessage(const String& message, const String& level) {
    return getCurrentDateTime() + "," + level + "," + message;
}

void DataLogger::writeToFile(const String& message) {
    File file = SD.open(LOG_DIRECTORY + "/log_0.txt", FILE_APPEND);
    if (file) {
        file.println(message);
        file.close();
        
        // Vérifier la taille du fichier et faire une rotation si nécessaire
        file = SD.open(LOG_DIRECTORY + "/log_0.txt");
        if (file.size() > 1024 * 1024) { // 1MB
            file.close();
            rotateLogFiles();
        } else {
            file.close();
        }
    }
}

bool DataLogger::checkAndCreateDirectory() {
    if (!SD.exists(LOG_DIRECTORY)) {
        return SD.mkdir(LOG_DIRECTORY);
    }
    return true;
}

String DataLogger::getCurrentDateTime() {
    struct tm timeinfo;
    char buffer[80];
    time_t now;
    
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    
    return String(buffer);
}