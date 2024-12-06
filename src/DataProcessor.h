#pragma once

#include "Config.h"
#include <vector>
#include <deque>

class DataProcessor {
public:
    DataProcessor();
    void begin();
    void process(const SensorData& data);
    DisplayData getDisplayData() const;
    void setMode(HuskyMode mode);
    
private:
    std::deque<HistoricalData> history;
    DisplayData displayData;
    HuskyMode currentMode;
    
    void processGestureData(const SensorData& data);
    void processDistanceData(const SensorData& data);
    void processMultiObjectData(const SensorData& data);
    void updateHistory(const SensorData& data);
    void analyzeData();
    std::vector<float> calculateTrends() const;
};
