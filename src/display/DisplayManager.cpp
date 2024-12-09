#include "DisplayManager.h"

DisplayManager::DisplayManager() {
}

void DisplayManager::begin() {
    M5.Lcd.begin();
    M5.Lcd.setRotation(Constants::Display::DEFAULT_ROTATION);
    clearScreen();
}

void DisplayManager::update() {
    // À implémenter plus tard
}

void DisplayManager::showMessage(const String& msg) {
    M5.Lcd.setTextColor(Constants::Colors::TEXT);
    M5.Lcd.drawString(msg, 10, 10);
}

void DisplayManager::clearScreen() {
    M5.Lcd.fillScreen(Constants::Colors::BACKGROUND);
}