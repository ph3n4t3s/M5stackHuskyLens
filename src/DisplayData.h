#pragma once

#include <Arduino.h>
#include <vector>
#include "Types.h"

struct DisplayData {
    std::vector<Point> points;
    std::vector<String> labels;
    std::vector<float> confidences;
    unsigned long timestamp;
    bool needsUpdate;
    String statusMessage;

    DisplayData() : 
        timestamp(millis()),
        needsUpdate(false),
        statusMessage("") {}
};