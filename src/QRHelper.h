#pragma once

#include <M5CoreS3.h>
#include <qrcode.h>

class QRHelper {
public:
    static void generateQR(const String& text, int x, int y, int scale = 3, uint32_t fgColor = TFT_BLACK, uint32_t bgColor = TFT_WHITE) {
        // Créer le QR code
        QRCode qrcode;
        uint8_t qrcodeData[qrcode_getBufferSize(3)];
        qrcode_initText(&qrcode, qrcodeData, 3, 0, text.c_str());

        // Dessiner le fond
        int size = qrcode.size * scale;
        M5.Lcd.fillRect(x - 4, y - 4, size + 8, size + 8, bgColor);

        // Dessiner le QR code
        for (uint8_t row = 0; row < qrcode.size; row++) {
            for (uint8_t col = 0; col < qrcode.size; col++) {
                if (qrcode_getModule(&qrcode, row, col)) {
                    M5.Lcd.fillRect(x + col * scale, y + row * scale, scale, scale, fgColor);
                }
            }
        }
    }

    static void generateWiFiQR(const String& ssid, const String& password, int x, int y, int scale = 3) {
        String qrData = "WIFI:S:" + ssid + ";T:WPA;P:" + password + ";;";
        generateQR(qrData, x, y, scale);
    }
};