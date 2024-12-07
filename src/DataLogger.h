#pragma once

#include <SD.h>
#include "Config.h"
#include <vector>
#include <deque>

class DataLogger {
public:
    DataLogger();
    bool begin();
    void log(const SensorData& data);
    void logError(const String& error);
    void logDebug(const String& message);
    void setLogLevel(int level);
    std::vector<String> getLastLogs(int count = 10);
    void clearLogs();
    bool exportCSV(const String& filename);
    
private:
    bool initialized;
    int logLevel;
    std::deque<String> logBuffer;
    const size_t MAX_BUFFER_SIZE = 1000;
    const String LOG_DIRECTORY = "/logs";
    
    void rotateLogFiles();
    String formatLogMessage(const String& message, const String& level);
    void writeToFile(const String& message);
    bool checkAndCreateDirectory();
    String getCurrentDateTime();
};