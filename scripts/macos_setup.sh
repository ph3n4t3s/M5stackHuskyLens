#!/bin/bash

echo "Configuration de l'environnement de développement pour macOS..."

# Vérifier si Homebrew est installé
if ! command -v brew &> /dev/null; then
    echo "Installation de Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

# Installer les dépendances nécessaires
echo "Installation des dépendances..."
brew install python3 git

# Installer PlatformIO
echo "Installation de PlatformIO..."
python3 -m pip install --upgrade pip
python3 -m pip install platformio

# Configuration initiale du projet
echo "Configuration du projet..."
pio platform install espressif32
pio pkg install

# Appliquer les patches de compatibilité
echo "Application des patches de compatibilité..."
bash scripts/apply_patches.sh

echo "Installation terminée !"
echo "Pour compiler le projet, exécutez : pio run"
echo "Pour téléverser le firmware, exécutez : pio run -t upload"