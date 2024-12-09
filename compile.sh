#!/bin/bash

# Nettoyage
rm -rf .pio/build/*

# Configuration de l'environnement
export PATH=$PATH:~/.platformio/penv/bin
export PLATFORMIO_CORE_DIR=/home/computeruse/.platformio

# Installation des dépendances
python -m pip install -U platformio
pio lib install \
    "m5stack/M5CoreS3" \
    "m5stack/M5Unified" \
    "huskylens/HuskyLens" \
    "bblanchon/ArduinoJson" \
    "fastled/FastLED" \
    "plerup/EspSoftwareSerial" \
    "lorol/LittleFS_esp32"

# Compilation
pio run -v