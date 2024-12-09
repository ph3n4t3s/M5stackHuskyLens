#pragma once

#include <M5CoreS3.h>
#include "Constants.h"

// Classe pour générer des QR codes simplifiés
class SimpleQR {
public:
    // Dessine un QR code WiFi simplifié
    static void drawWiFiQR(const String& ssid, const String& password, int x, int y) {
        using namespace Constants;
        const int cellSize = Display::QR_CODE_SCALE;
        const int size = Display::QR_CODE_SIZE;
        const int totalSize = size * cellSize;
        
        // Fond blanc avec bordure
        M5.Lcd.fillRect(x - 5, y - 5, totalSize + 10, totalSize + 10, Colors::TEXT);
        M5.Lcd.drawRect(x - 5, y - 5, totalSize + 10, totalSize + 10, Colors::BACKGROUND);
        
        // Symbole WiFi
        drawWiFiSymbol(x + totalSize/2, y - 15);
        
        // Motifs de position (coins)
        drawPositionPattern(x, y, cellSize);
        drawPositionPattern(x + totalSize - cellSize * 3, y, cellSize);
        drawPositionPattern(x, y + totalSize - cellSize * 3, cellSize);
        
        // Données (motif basé sur SSID et mot de passe)
        String data = ssid + password;
        for (int i = 0; i < size - 3; i++) {
            for (int j = 0; j < size - 3; j++) {
                if (shouldFillCell(data, i, j)) {
                    int px = x + (i + 3) * cellSize;
                    int py = y + (j + 3) * cellSize;
                    M5.Lcd.fillRect(px, py, cellSize - 1, cellSize - 1, Colors::BACKGROUND);
                }
            }
        }
        
        // SSID sous le QR code
        M5.Lcd.setTextSize(1);
        M5.Lcd.setTextColor(Colors::BACKGROUND);
        M5.Lcd.drawString("SSID: " + ssid, x, y + totalSize + 7);
    }

private:
    // Dessine un motif de position (carré avec point au centre)
    static void drawPositionPattern(int x, int y, int cellSize) {
        using namespace Constants::Colors;
        // Carré extérieur
        M5.Lcd.fillRect(x, y, cellSize * 3, cellSize * 3, BACKGROUND);
        // Carré intérieur
        M5.Lcd.fillRect(x + cellSize, y + cellSize, cellSize, cellSize, TEXT);
    }
    
    // Dessine un symbole WiFi simplifié
    static void drawWiFiSymbol(int x, int y) {
        uint32_t color = Constants::Colors::BACKGROUND;
        // Arc externe
        M5.Lcd.drawCircle(x, y + 10, 12, color);
        // Arc moyen
        M5.Lcd.drawCircle(x, y + 10, 8, color);
        // Point central
        M5.Lcd.fillCircle(x, y + 10, 3, color);
    }
    
    // Détermine si une cellule doit être remplie (pseudo-aléatoire)
    static bool shouldFillCell(const String& data, int i, int j) {
        // Crée un motif pseudo-aléatoire basé sur les données
        int index = (i + j) % data.length();
        char c = data[index];
        return ((c + i + j) % 3 == 0);
    }
};