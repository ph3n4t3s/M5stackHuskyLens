#pragma once

#include <M5CoreS3.h>
#include "Config.h"
#include <vector>

class DisplayManager {
public:
    DisplayManager();
    void begin();
    void update(const DisplayData& data);
    void setMode(DisplayMode mode);
    void showError(const String& message, bool isRecoverable = true);
    void showMenu(const std::vector<String>& items, int selectedIndex);
    
private:
    DisplayMode currentMode;
    bool nightMode;
    
    void drawInterface(const DisplayData& data);
    void drawAnalytics(const DisplayData& data);
    void drawDebugInfo(const DisplayData& data);
    void drawStatusBar(const String& status);
    void drawDetectionZones(const std::vector<DetectionZone>& zones);
    void drawGraph(const std::vector<float>& data, int x, int y, int width, int height);
    void clearScreen();
};
