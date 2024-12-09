#pragma once

#include <M5CoreS3.h>
#include "../core/Constants.h"
#include "../core/Types.h"

class DisplayManager {
public:
    DisplayManager();
    
    // Initialisation et configuration
    void begin();
    void setBrightness(uint8_t value);
    void setTextSize(uint8_t size);
    
    // Opérations d'affichage
    void update();
    void showMessage(const String& msg, uint32_t color = Constants::Colors::TEXT);
    void showCenteredMessage(const String& msg, int y, uint32_t color = Constants::Colors::TEXT);
    void clearScreen();
    
    // État de l'affichage
    bool isReady() const { return initialized; }
    uint8_t getBrightness() const { return brightness; }
    uint8_t getTextSize() const { return textSize; }
    
private:
    bool initialized;
    uint8_t brightness;
    uint8_t textSize;
    
    // Utilitaires
    int calculateTextWidth(const String& text) const;
    void updateBrightness();
};
};