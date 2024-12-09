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

void DisplayManager::showQRCode(bool show) {
    clearScreen();
    if (show) {
        M5.Lcd.drawString("Scan QR Code", Constants::Display::SCREEN_WIDTH/2 - 50, Constants::Display::SCREEN_HEIGHT/2);
    }
}

void DisplayManager::setMode(DisplayMode mode) {
    clearScreen();
    switch (mode) {
        case DisplayMode::SETTINGS:
            M5.Lcd.drawString("Settings Mode", 10, 10);
            break;
        default:
            break;
    }
}

void DisplayManager::showError(const String& message, bool recoverable) {
    clearScreen();
    M5.Lcd.setTextColor(Constants::Colors::ERROR);
    M5.Lcd.drawString(message, 10, Constants::Display::SCREEN_HEIGHT/2);
}

void DisplayManager::showBootSequence(const String& message) {
    clearScreen();
    M5.Lcd.setTextColor(Constants::Colors::INFO);
    M5.Lcd.drawString(message, 10, Constants::Display::SCREEN_HEIGHT/2);
}

void DisplayManager::setBrightness(uint8_t value) {
    M5.Lcd.setBrightness(value);
}

void DisplayManager::generateWifiQR(const char* ssid, const char* password) {
    String qrData = "WIFI:S:" + String(ssid) + ";T:WPA;P:" + String(password) + ";;";
    clearScreen();
    M5.Lcd.qrcode(qrData.c_str(), 60, 20, 200, 6);
    M5.Lcd.drawString("SSID: " + String(ssid), 10, Constants::Display::SCREEN_HEIGHT - 40);
    M5.Lcd.drawString("Pass: " + String(password), 10, Constants::Display::SCREEN_HEIGHT - 20);
}