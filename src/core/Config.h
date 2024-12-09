#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "Constants.h"
#include "Types.h"

class Config {
public:
    // Configuration générale
    SystemConfig system;
    DisplayConfig display;
    WiFiConfig wifi;
    
    // Gestion EEPROM
    static const int EEPROM_SIZE = 512;
    static const int CONFIG_VERSION = 2;  // Incrémenter si la structure change
    
    // Initialisation
    void begin() {
        EEPROM.begin(EEPROM_SIZE);
        loadDefaults();
        if (!loadFromEEPROM()) {
            saveToEEPROM();  // Sauver les valeurs par défaut
        }
    }
    
    // Sauvegarde
    bool saveToEEPROM() {
        DynamicJsonDocument doc(384);  // Ajuster selon la taille nécessaire
        
        // Version
        doc["version"] = CONFIG_VERSION;
        
        // Système
        JsonObject sys = doc.createNestedObject("system");
        sys["deviceName"] = system.deviceName;
        sys["volume"] = system.volume;
        sys["debugMode"] = system.debugMode;
        
        // Affichage
        JsonObject disp = doc.createNestedObject("display");
        disp["brightness"] = display.brightness;
        disp["nightMode"] = display.nightMode;
        disp["rotation"] = display.rotation;
        
        // WiFi
        JsonObject wf = doc.createNestedObject("wifi");
        wf["ssid"] = wifi.ssid;
        wf["autoConnect"] = wifi.autoConnect;
        
        // Ne pas sauvegarder le mot de passe en clair dans l'EEPROM
        
        String output;
        serializeJson(doc, output);
        
        for (size_t i = 0; i < output.length(); i++) {
            EEPROM.write(i, output[i]);
        }
        EEPROM.write(output.length(), 0);  // Marqueur de fin
        
        return EEPROM.commit();
    }
    
    // Chargement
    bool loadFromEEPROM() {
        String data;
        for (int i = 0; i < EEPROM_SIZE; i++) {
            char c = EEPROM.read(i);
            if (c == 0) break;
            data += c;
        }
        
        DynamicJsonDocument doc(384);
        DeserializationError error = deserializeJson(doc, data);
        
        if (error || !doc.containsKey("version") || doc["version"] != CONFIG_VERSION) {
            return false;
        }
        
        // Système
        if (doc.containsKey("system")) {
            JsonObject sys = doc["system"];
            system.deviceName = sys["deviceName"].as<String>();
            system.volume = sys["volume"] | Constants::System::DEFAULT_VOLUME;
            system.debugMode = sys["debugMode"] | false;
        }
        
        // Affichage
        if (doc.containsKey("display")) {
            JsonObject disp = doc["display"];
            display.brightness = disp["brightness"] | Constants::Display::DEFAULT_BRIGHTNESS;
            display.nightMode = disp["nightMode"] | false;
            display.rotation = disp["rotation"] | Constants::Display::DEFAULT_ROTATION;
        }
        
        // WiFi
        if (doc.containsKey("wifi")) {
            JsonObject wf = doc["wifi"];
            strlcpy(wifi.ssid, wf["ssid"] | "", sizeof(wifi.ssid));
            wifi.autoConnect = wf["autoConnect"] | true;
        }
        
        return true;
    }

private:
    // Valeurs par défaut
    void loadDefaults() {
        system = SystemConfig();
        display = DisplayConfig();
        wifi = WiFiConfig();
    }
};