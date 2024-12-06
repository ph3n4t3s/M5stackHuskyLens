#include "DisplayManager.h"

DisplayManager::DisplayManager() : 
    currentMode(DisplayMode::GRAPHIC_INTERFACE),
    nightMode(false) {}

void DisplayManager::begin() {
    M5.Lcd.setTextSize(2);
    clearScreen();
}

void DisplayManager::update(const DisplayData& data) {
    if (!data.needsUpdate) return;
    
    clearScreen();
    drawStatusBar(data.statusMessage);
    
    switch (currentMode) {
        case DisplayMode::RAW_DATA:
            drawInterface(data);
            break;
        case DisplayMode::ANALYTICS_VIEW:
            drawAnalytics(data);
            break;
        case DisplayMode::DEBUGGING_VIEW:
            drawDebugInfo(data);
            break;
        default:
            drawInterface(data);
    }
}

void DisplayManager::drawInterface(const DisplayData& data) {
    for (size_t i = 0; i < data.points.size(); i++) {
        const Point& p = data.points[i];
        M5.Lcd.fillCircle(p.x, p.y, 3, TFT_GREEN);
        
        if (i < data.labels.size()) {
            M5.Lcd.drawString(data.labels[i], p.x + 5, p.y - 10);
        }
        
        if (i < data.confidences.size()) {
            M5.Lcd.drawFloat(data.confidences[i], 1, p.x + 5, p.y + 10);
        }
    }
}

void DisplayManager::drawAnalytics(const DisplayData& data) {
    const int GRAPH_X = 10;
    const int GRAPH_Y = 50;
    const int GRAPH_W = 300;
    const int GRAPH_H = 150;
    
    M5.Lcd.drawRect(GRAPH_X, GRAPH_Y, GRAPH_W, GRAPH_H, TFT_WHITE);
    
    std::vector<float> graphData;
    for (float conf : data.confidences) {
        graphData.push_back(conf);
    }
    
    drawGraph(graphData, GRAPH_X, GRAPH_Y, GRAPH_W, GRAPH_H);
}

void DisplayManager::drawGraph(const std::vector<float>& data, int x, int y, int width, int height) {
    if (data.empty()) return;
    
    float maxVal = *std::max_element(data.begin(), data.end());
    float minVal = *std::min_element(data.begin(), data.end());
    float range = maxVal - minVal;
    if (range == 0) range = 1;
    
    for (size_t i = 1; i < data.size(); i++) {
        int x1 = x + (width * (i-1)) / data.size();
        int x2 = x + (width * i) / data.size();
        int y1 = y + height - (height * (data[i-1] - minVal)) / range;
        int y2 = y + height - (height * (data[i] - minVal)) / range;
        
        M5.Lcd.drawLine(x1, y1, x2, y2, TFT_YELLOW);
    }
}

void DisplayManager::drawDebugInfo(const DisplayData& data) {
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf("Points: %d\n", data.points.size());
    M5.Lcd.printf("Labels: %d\n", data.labels.size());
    M5.Lcd.printf("Conf: %d\n", data.confidences.size());
    M5.Lcd.println(data.statusMessage);
}

void DisplayManager::drawStatusBar(const String& status) {
    M5.Lcd.fillRect(0, 0, 320, 20, TFT_BLUE);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.drawString(status, 5, 5);
    M5.Lcd.setTextColor(nightMode ? TFT_WHITE : TFT_BLACK);
}

void DisplayManager::showMenu(const std::vector<String>& items, int selectedIndex) {
    clearScreen();
    M5.Lcd.setTextSize(2);
    
    for (size_t i = 0; i < items.size(); i++) {
        int y = 30 + i * 30;
        if (i == selectedIndex) {
            M5.Lcd.fillRect(0, y - 2, 320, 25, TFT_BLUE);
            M5.Lcd.setTextColor(TFT_WHITE);
        } else {
            M5.Lcd.setTextColor(nightMode ? TFT_WHITE : TFT_BLACK);
        }
        M5.Lcd.drawString(items[i], 10, y);
    }
}

void DisplayManager::showError(const String& message, bool isRecoverable) {
    uint32_t bgColor = isRecoverable ? TFT_YELLOW : TFT_RED;
    M5.Lcd.fillScreen(bgColor);
    M5.Lcd.setTextColor(TFT_BLACK);
    // Utilisation de setCursor et print au lieu de drawString
    M5.Lcd.setCursor(10, M5.Lcd.height()/2 - 30);
    M5.Lcd.setTextSize(4);
    M5.Lcd.print(message);
    M5.Lcd.setTextSize(2);  // Retour à la taille par défaut
}

void DisplayManager::clearScreen() {
    M5.Lcd.fillScreen(nightMode ? TFT_BLACK : TFT_WHITE);
    M5.Lcd.setTextColor(nightMode ? TFT_WHITE : TFT_BLACK);
}

void DisplayManager::setMode(DisplayMode mode) {
    if (currentMode != mode) {
        currentMode = mode;
        clearScreen();
    }
}
