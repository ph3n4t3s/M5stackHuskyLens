#pragma once

#include <M5CoreS3.h>
#include "Config.h"
#include <vector>
#include <queue>
#include <qrcode.h>

enum class BootStage {
    INIT,
    HARDWARE_CHECK,
    WIFI_SETUP,
    MQTT_SETUP,
    COMPLETE
};

enum class DisplayMode {
    BOOT_SEQUENCE,
    SETUP_QR,
    NORMAL_OPERATION,
    MENU,
    SETTINGS,
    ERROR
};

struct BootMessage {
    String message;
    uint32_t color;
    unsigned long timestamp;
};

class DisplayManager {
public:
    DisplayManager();
    void begin();
    void update();
    
    // Gestion du démarrage
    void showBootSequence(BootStage stage, const String& message);
    void addBootLog(const String& message, uint32_t color = TFT_WHITE);
    
    // QR Code
    void generateWifiQR(const String& ssid, const String& password);
    void showQRCode(bool show = true);
    void toggleQRCode();
    
    // Interface utilisateur
    void showMainInterface();
    void showSettings();
    void showMenu(const std::vector<String>& items, int selectedIndex);
    void showError(const String& message, bool isRecoverable = true);
    
    // Gestion de l'affichage
    void setMode(DisplayMode mode);
    void setBrightness(uint8_t brightness);
    bool isQRVisible() const { return qrVisible; }
    
private:
    DisplayMode currentMode;
    bool nightMode;
    bool qrVisible;
    std::queue<BootMessage> bootLog;
    uint8_t* qrcode;
    
    // Configuration d'affichage
    static const int SCREEN_WIDTH = 320;
    static const int SCREEN_HEIGHT = 240;
    static const int FONT_SMALL = 2;
    static const int FONT_MEDIUM = 3;
    static const int FONT_LARGE = 4;
    
    // Méthodes privées d'affichage
    void drawBootLog();
    void drawQRCode();
    void drawStatusBar(const String& status);
    void drawMainInterface();
    void drawSettingsInterface();
    void clearScreen();
    
    // Utilitaires
    void setFont(int size);
    int calculateTextWidth(const String& text, int fontSize);
    void drawCenteredText(const String& text, int y, int fontSize);
};