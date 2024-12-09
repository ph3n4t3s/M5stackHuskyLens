#!/bin/bash

# Couleurs pour le terminal
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Vérification de l'environnement
echo -e "${YELLOW}Vérification de l'environnement...${NC}"
python -m platformio --version
if [ $? -ne 0 ]; then
    echo -e "${RED}PlatformIO n'est pas installé !${NC}"
    exit 1
fi

# Nettoyage
echo -e "${YELLOW}Nettoyage des builds précédents...${NC}"
rm -rf .pio/build/*

# Compilation et exécution des tests
echo -e "${YELLOW}Compilation des tests...${NC}"
pio run -e test
if [ $? -ne 0 ]; then
    echo -e "${RED}Erreur de compilation !${NC}"
    exit 1
fi

# Upload et exécution des tests sur la carte
echo -e "${YELLOW}Upload des tests...${NC}"
pio test -e test --upload-port /dev/ttyUSB0
if [ $? -ne 0 ]; then
    echo -e "${RED}Erreur pendant les tests !${NC}"
    exit 1
fi

echo -e "${GREEN}Tous les tests ont réussi !${NC}"