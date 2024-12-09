#!/bin/bash
set -e

echo "Configuration de l'environnement PlatformIO..."
mkdir -p ~/.platformio
platformio platform install espressif32

echo "Installation des dépendances du projet..."
platformio lib install

echo "Nettoyage des fichiers de build précédents..."
rm -rf .pio/build/*

echo "Compilation du projet..."
platformio run -v

# Si la compilation réussit, on copie les fichiers dans le dossier de sortie
if [ $? -eq 0 ]; then
    echo "Compilation réussie !"
    mkdir -p /workspace/output
    cp .pio/build/esp32-s3-dev/firmware.bin /workspace/output/
    cp .pio/build/esp32-s3-dev/partitions.bin /workspace/output/
    echo "Les fichiers de sortie ont été copiés dans /workspace/output/"
else
    echo "Erreur lors de la compilation !"
    exit 1
fi