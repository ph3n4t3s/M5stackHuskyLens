#pragma once

#include <Arduino.h>
#include <vector>
#include <EEPROM.h>

// Tailles de l'écran M5Stack Core S3
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Couleurs personnalisées
#define COLOR_BACKGROUND  0x0000  // Noir
#define COLOR_TEXT       0xFFFF   // Blanc
#define COLOR_PRIMARY    0x04FF   // Bleu
#define COLOR_SUCCESS    0x07E0   // Vert
#define COLOR_WARNING    0xFFE0   // Jaune
#define COLOR_ERROR      0xF800   // Rouge
#define COLOR_INFO       0x07FF   // Cyan

// Adresses EEPROM pour les paramètres
#define EEPROM_WIFI_CONFIG     0
#define EEPROM_DISPLAY_CONFIG  64
#define EEPROM_SYSTEM_CONFIG   128

// Structures de configuration
struct WifiConfig {
    char ssid[32];
    char password[64];
    bool configured;
    
    void save() {
        EEPROM.put(EEPROM_WIFI_CONFIG, *this);
        EEPROM.commit();
    }
    
    void load() {
        EEPROM.get(EEPROM_WIFI_CONFIG, *this);
    }
};

struct DisplayConfig {
    uint8_t brightness;
    bool nightMode;
    uint8_t textSize;
    uint16_t backgroundColor;
    uint16_t textColor;
    
    void save() {
        EEPROM.put(EEPROM_DISPLAY_CONFIG, *this);
        EEPROM.commit();
    }
    
    void load() {
        EEPROM.get(EEPROM_DISPLAY_CONFIG, *this);
    }
};

struct SystemConfig {
    bool firstBoot;
    char deviceName[32];
    uint8_t volume;
    bool debugMode;
    
    void save() {
        EEPROM.put(EEPROM_SYSTEM_CONFIG, *this);
        EEPROM.commit();
    }
    
    void load() {
        EEPROM.get(EEPROM_SYSTEM_CONFIG, *this);
    }
};

// Structure pour les données d'affichage
struct DisplayData {
    bool needsUpdate;
    String statusMessage;
    std::vector<String> bootMessages;
    std::vector<uint32_t> bootMessageColors;
    unsigned long bootStartTime;
    int bootProgress;
};

// Structure pour la détection d'objets
struct DetectionObject {
    int id;
    String label;
    float confidence;
    int x;
    int y;
    int width;
    int height;
};

// Constantes pour le système
const int MAX_BOOT_MESSAGES = 8;
const int MAX_DETECTION_OBJECTS = 10;
const int BOOT_TIMEOUT = 30000; // 30 secondes
const int QR_CODE_TIMEOUT = 300000; // 5 minutes

// Configuration par défaut
const DisplayConfig DEFAULT_DISPLAY_CONFIG = {
    .brightness = 128,        // 50% luminosité
    .nightMode = false,
    .textSize = 2,
    .backgroundColor = COLOR_BACKGROUND,
    .textColor = COLOR_TEXT
};

const SystemConfig DEFAULT_SYSTEM_CONFIG = {
    .firstBoot = true,
    .deviceName = "M5HuskyLens",
    .volume = 128,
    .debugMode = false
};

// Classes de configuration
class ConfigManager {
public:
    static WifiConfig wifiConfig;
    static DisplayConfig displayConfig;
    static SystemConfig systemConfig;
    
    static void begin() {
        EEPROM.begin(512);
        loadAllConfigs();
    }
    
    static void loadAllConfigs() {
        wifiConfig.load();
        displayConfig.load();
        systemConfig.load();
        
        // Vérification et initialisation des paramètres par défaut si nécessaire
        if (systemConfig.firstBoot) {
            resetToDefaults();
        }
    }
    
    static void resetToDefaults() {
        displayConfig = DEFAULT_DISPLAY_CONFIG;
        systemConfig = DEFAULT_SYSTEM_CONFIG;
        systemConfig.firstBoot = false;
        
        displayConfig.save();
        systemConfig.save();
    }
    
    static void saveAllConfigs() {
        wifiConfig.save();
        displayConfig.save();
        systemConfig.save();
    }
};

// Définitions des messages du système
const char* const BOOT_MESSAGES[] = {
    "Initialisation du système...",
    "Vérification du matériel...",
    "Configuration du WiFi...",
    "Connexion à HuskyLens...",
    "Initialisation de l'affichage...",
    "Configuration du système...",
    "Chargement des paramètres...",
    "Système prêt !"
};

// États du système
enum class SystemState {
    BOOTING,
    SETUP_REQUIRED,
    CONNECTING_WIFI,
    CONNECTING_HUSKYLENS,
    READY,
    ERROR,
    SETTINGS
};

// Types d'erreurs
enum class ErrorType {
    NONE,
    WIFI_ERROR,
    HUSKYLENS_ERROR,
    SYSTEM_ERROR,
    HARDWARE_ERROR
};