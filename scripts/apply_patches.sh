#!/bin/bash

# Script pour appliquer les patches de compatibilité
echo "Application des patches de compatibilité..."

# Vérifier si le dossier des dépendances existe
if [ ! -d ".pio/libdeps" ]; then
    echo "Erreur : Dossier .pio/libdeps non trouvé."
    echo "Veuillez d'abord installer les dépendances avec 'pio pkg install'"
    exit 1
fi

# Appliquer le patch de compatibilité MacOS
patch -p1 < patches/mac_compatibility.patch

echo "Patches appliqués avec succès !"