#include "DisplayManager.h"

DisplayManager::DisplayManager() : 
    currentMode(DisplayMode::BOOT_SEQUENCE),
    nightMode(false),
    qrVisible(false),
    qrcode(nullptr) {}

void DisplayManager::begin() {
    M5.Lcd.begin();
    M5.Lcd.setRotation(0);
    M5.Lcd.fillScreen(TFT_BLACK);
    setBrightness(128); // 50% brightness par défaut
}

void DisplayManager::update() {
    switch (currentMode) {
        case DisplayMode::BOOT_SEQUENCE:
            drawBootLog();
            break;
        case DisplayMode::SETUP_QR:
            drawQRCode();
            break;
        case DisplayMode::NORMAL_OPERATION:
            drawMainInterface();
            break;
        case DisplayMode::SETTINGS:
            drawSettingsInterface();
            break;
        default:
            break;
    }
}

void DisplayManager::showBootSequence(BootStage stage, const String& message) {
    uint32_t color;
    String stageText;
    
    switch (stage) {
        case BootStage::INIT:
            stageText = "Initialisation";
            color = TFT_BLUE;
            break;
        case BootStage::HARDWARE_CHECK:
            stageText = "Vérification matériel";
            color = TFT_GREEN;
            break;
        case BootStage::WIFI_SETUP:
            stageText = "Configuration WiFi";
            color = TFT_YELLOW;
            break;
        case BootStage::MQTT_SETUP:
            stageText = "Configuration MQTT";
            color = TFT_MAGENTA;
            break;
        case BootStage::COMPLETE:
            stageText = "Démarrage terminé";
            color = TFT_WHITE;
            break;
    }
    
    addBootLog(stageText + ": " + message, color);
}

void DisplayManager::addBootLog(const String& message, uint32_t color) {
    BootMessage bootMsg = {
        message,
        color,
        millis()
    };
    
    bootLog.push(bootMsg);
    if (bootLog.size() > 8) { // Garde les 8 derniers messages
        bootLog.pop();
    }
    
    if (currentMode == DisplayMode::BOOT_SEQUENCE) {
        drawBootLog();
    }
}

void DisplayManager::generateWifiQR(const String& ssid, const String& password) {
    String qrData = "WIFI:S:" + ssid + ";T:WPA;P:" + password + ";;";
    
    // Création du QR code
    QRCode qr;
    const int qrVersion = 5; // Version 5 pour un QR code de taille moyenne
    
    if (qrcode != nullptr) {
        delete[] qrcode;
    }
    
    qrcode = new uint8_t[qr.getBufferSize(qrVersion)];
    qr.init(qrVersion, 0);
    qr.encode(qrData.c_str());
}

void DisplayManager::showQRCode(bool show) {
    qrVisible = show;
    if (show) {
        drawQRCode();
    } else {
        drawMainInterface();
    }
}

void DisplayManager::toggleQRCode() {
    showQRCode(!qrVisible);
}

void DisplayManager::drawBootLog() {
    clearScreen();
    
    // Dessiner le titre
    setFont(FONT_MEDIUM);
    drawCenteredText("Séquence de démarrage", 10, FONT_MEDIUM);
    
    // Dessiner les messages de log
    setFont(FONT_SMALL);
    int y = 50;
    std::queue<BootMessage> tempQueue = bootLog;
    
    while (!tempQueue.empty()) {
        BootMessage msg = tempQueue.front();
        tempQueue.pop();
        
        M5.Lcd.setTextColor(msg.color);
        M5.Lcd.setCursor(10, y);
        M5.Lcd.print(msg.message);
        y += 20;
    }
}

void DisplayManager::drawQRCode() {
    if (!qrcode) return;
    
    clearScreen();
    setFont(FONT_SMALL);
    drawCenteredText("Scanner pour configurer le WiFi", 10, FONT_SMALL);
    
    // Calculer la position centrale pour le QR code
    const int scale = 4; // Échelle du QR code
    QRCode qr;
    const int size = qr.getSize();
    const int qrX = (SCREEN_WIDTH - (size * scale)) / 2;
    const int qrY = (SCREEN_HEIGHT - (size * scale)) / 2;
    
    // Dessiner le QR code
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (qr.getModule(x, y)) {
                M5.Lcd.fillRect(qrX + x*scale, qrY + y*scale, scale, scale, TFT_WHITE);
            }
        }
    }
}

void DisplayManager::drawMainInterface() {
    clearScreen();
    drawStatusBar("En fonctionnement");
    
    // Si le QR code est visible, le superposer
    if (qrVisible && qrcode) {
        drawQRCode();
    }
}

void DisplayManager::drawSettingsInterface() {
    clearScreen();
    setFont(FONT_MEDIUM);
    drawCenteredText("Paramètres", 10, FONT_MEDIUM);
    
    setFont(FONT_SMALL);
    M5.Lcd.setCursor(10, 50);
    M5.Lcd.println("Appuyer sur :");
    M5.Lcd.println("A: Afficher QR Code");
    M5.Lcd.println("B: Changer la luminosité");
    M5.Lcd.println("C: Mode nuit");
}

void DisplayManager::drawStatusBar(const String& status) {
    M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, 30, TFT_BLUE);
    setFont(FONT_SMALL);
    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.drawString(status, 5, 5);
}

void DisplayManager::showMenu(const std::vector<String>& items, int selectedIndex) {
    clearScreen();
    setFont(FONT_SMALL);
    
    for (size_t i = 0; i < items.size(); i++) {
        int y = 40 + i * 30;
        if (i == selectedIndex) {
            M5.Lcd.fillRect(0, y - 2, SCREEN_WIDTH, 25, TFT_BLUE);
            M5.Lcd.setTextColor(TFT_WHITE);
        } else {
            M5.Lcd.setTextColor(nightMode ? TFT_WHITE : TFT_BLACK);
        }
        M5.Lcd.drawString(items[i], 10, y);
    }
}

void DisplayManager::showError(const String& message, bool isRecoverable) {
    uint32_t bgColor = isRecoverable ? TFT_YELLOW : TFT_RED;
    clearScreen();
    M5.Lcd.fillScreen(bgColor);
    setFont(FONT_MEDIUM);
    M5.Lcd.setTextColor(TFT_BLACK);
    drawCenteredText(message, SCREEN_HEIGHT/2 - 20, FONT_MEDIUM);
}

void DisplayManager::setMode(DisplayMode mode) {
    if (currentMode != mode) {
        currentMode = mode;
        clearScreen();
    }
}

void DisplayManager::setBrightness(uint8_t brightness) {
    // 0-255
    // TODO: Implémenter le contrôle de la luminosité selon le matériel
}

void DisplayManager::clearScreen() {
    M5.Lcd.fillScreen(nightMode ? TFT_BLACK : TFT_WHITE);
    M5.Lcd.setTextColor(nightMode ? TFT_WHITE : TFT_BLACK);
}

void DisplayManager::setFont(int size) {
    M5.Lcd.setTextSize(size);
}

int DisplayManager::calculateTextWidth(const String& text, int fontSize) {
    return text.length() * 6 * fontSize; // Approximation
}

void DisplayManager::drawCenteredText(const String& text, int y, int fontSize) {
    int textWidth = calculateTextWidth(text, fontSize);
    int x = (SCREEN_WIDTH - textWidth) / 2;
    M5.Lcd.setCursor(x, y);
    M5.Lcd.print(text);
}