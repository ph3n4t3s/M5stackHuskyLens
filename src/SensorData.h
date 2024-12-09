#pragma once

#include <Arduino.h>
#include <vector>
#include "Types.h"

struct SensorData {
    std::vector<Point> points;
    std::vector<String> labels;
    float confidence;
    unsigned long timestamp;
    int objectCount;

    SensorData() : confidence(0.0f), timestamp(0), objectCount(0) {}
};