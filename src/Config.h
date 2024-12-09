#pragma once

#include <Arduino.h>
#include "Constants.h"
#include "Types.h"
#include <vector>

// Configuration du projet
const char* const PROJECT_NAME = "M5stackHuskyLens";
const char* const PROJECT_VERSION = "2.0.0";

// Configuration WiFi par défaut
const char* const DEFAULT_AP_SSID = "M5HuskyLens";
const char* const DEFAULT_AP_PASSWORD = "12345678";

// Configuration des pins
#define PIN_HUSKYLENS_TX 2
#define PIN_HUSKYLENS_RX 1

// Configuration de la communication série
#define SERIAL_BAUD_RATE 115200
#define HUSKYLENS_BAUD_RATE 9600

// Timeouts et délais
const unsigned long WIFI_TIMEOUT = 30000;        // 30 secondes
const unsigned long HUSKYLENS_TIMEOUT = 5000;    // 5 secondes
const unsigned long BOOT_DISPLAY_TIME = 2000;    // 2 secondes

// Configuration de l'affichage
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;
const int MAX_BRIGHTNESS = 255;

// Configuration des boutons
const uint8_t BUTTON_A_PIN = 39;
const uint8_t BUTTON_B_PIN = 38;
const uint8_t BUTTON_C_PIN = 37;

// Paramètres de détection
const float MIN_CONFIDENCE = 0.6f;
const int MAX_DETECTIONS = 10;

// Paramètres de débogage
#ifdef DEBUG
    #define DEBUG_PRINT(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
#endif