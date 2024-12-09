#!/bin/bash

# Installation des dépendances
echo "Installation des dépendances..."

# Création du répertoire lib s'il n'existe pas
mkdir -p lib

# Installation de QRCode
if [ ! -d "lib/QRCode" ]; then
    echo "Installation de QRCode..."
    mkdir -p lib/QRCode
    cd lib/QRCode
    curl -L https://github.com/ricmoo/QRCode/archive/refs/heads/master.zip -o qrcode.zip
    unzip qrcode.zip
    mv QRCode-master/* .
    rm -rf QRCode-master qrcode.zip
    cd ../..
fi

# Mise à jour du fichier platformio.ini si nécessaire
if ! grep -q "ricmoo/QRCode" platformio.ini; then
    echo "Ajout de la dépendance QRCode à platformio.ini..."
    sed -i '/lib_deps =/a \        ricmoo/QRCode@^0.0.1' platformio.ini
fi

echo "Installation terminée."