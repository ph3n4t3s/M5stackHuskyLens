# Spécifications Techniques - M5Stack HuskyLens v1.1.0

## Vue d'ensemble du projet

Le projet M5Stack HuskyLens est une solution intégrée de vision par ordinateur combinant le M5Stack CoreS3 et le module HuskyLens. Cette solution permet la reconnaissance d'objets, de visages, et l'analyse en temps réel de diverses caractéristiques visuelles.

## Architecture du système

### Composants matériels
- **M5Stack CoreS3**
  - Processeur : ESP32-S3
  - RAM : 512KB SRAM + 2MB PSRAM
  - Stockage : Flash 16MB
  - Écran : LCD 320×240 IPS
  - Communication : WiFi, Bluetooth, I2C, UART
  
- **HuskyLens**
  - Processeur : K210
  - Caméra : OV2640
  - Interface : I2C/UART
  - Algorithmes intégrés : reconnaissance faciale, détection d'objets, suivi de ligne

### Structure logicielle

```
src/
├── analytics/         # Gestion des analytics et métriques
├── config/           # Configuration système
├── core/            # Fonctionnalités core
├── handlers/        # Gestionnaires d'événements
├── utils/           # Utilitaires communs
└── web/            # Interface web et API REST
```

### Modules principaux

1. **DisplayManager**
   - Gestion de l'affichage LCD
   - Interface utilisateur locale
   - Rendu des données en temps réel

2. **MLSystem**
   - Interface avec HuskyLens
   - Gestion des algorithmes de ML
   - Configuration des modes de reconnaissance

3. **WiFiManager**
   - Configuration WiFi
   - Point d'accès local
   - Gestion des connexions

4. **AnalyticsManager**
   - Collecte de métriques
   - Analyse des performances
   - Stockage des données

5. **WebInterface**
   - API REST
   - Interface de configuration web
   - Streaming de données en temps réel

## Protocoles et interfaces

### Communication HuskyLens
- Protocol : I2C
- Adresse : 0x32
- Vitesse : 100kHz
- Format des données : Protocol personnalisé (voir HUSKYLENS.h)

### API REST
- Format : JSON
- Endpoints principaux :
  - `/api/v1/status` : État du système
  - `/api/v1/config` : Configuration
  - `/api/v1/data` : Données en temps réel

### Interface Web
- Port : 80
- Protocole : HTTP
- Framework : ESPAsyncWebServer
- Fonctionnalités :
  - Configuration en direct
  - Visualisation des données
  - Mise à jour firmware

## Configuration système

### Paramètres PlatformIO
\`\`\`ini
[env:esp32-s3-dev]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
board_build.flash_size = 16MB
board_build.psram = enabled
\`\`\`

### Dépendances principales
- M5Unified
- HuskyLens
- ESPAsyncWebServer
- ArduinoJson
- FastLED

## Modes de fonctionnement

1. **Mode Standalone**
   - Interface utilisateur locale
   - Configuration via boutons
   - Stockage local des données

2. **Mode Connected**
   - Interface web active
   - Streaming de données
   - Configuration à distance

3. **Mode Learning**
   - Apprentissage de nouveaux objets
   - Calibration des algorithmes
   - Sauvegarde des modèles

## Considérations de performance

### Utilisation mémoire
- RAM : ~200KB base
- PSRAM : ~1MB pour le traitement d'image
- Flash : ~4MB programme + données

### Temps de réponse
- Détection : <100ms
- Classification : <200ms
- API REST : <50ms

## Particularités par plateforme

### Linux
- Compilation standard
- Utilisation de tools standards

### macOS (Intel/M1/M2)
- Patches spécifiques pour HuskyLens
- Gestion des types pour la compilation
- Scripts de correction automatique

### Windows
- Drivers USB spécifiques requis
- Compilation avec MinGW

## Sécurité

### Communication
- Chiffrement WiFi WPA2
- API tokens pour l'accès REST
- Validation des entrées

### Mise à jour
- OTA sécurisé
- Vérification des signatures
- Rollback automatique

## Maintenance et développement

### Compilation
\`\`\`bash
# Installation des dépendances
pio pkg install

# Compilation
pio run

# Upload
pio run --target upload
\`\`\`

### Tests
\`\`\`bash
# Tests unitaires
pio test

# Tests d'intégration
pio test -e integration
\`\`\`

### Debug
- Port série : 115200 baud
- Niveaux de log configurables
- Moniteur web intégré

## Extensions futures

1. **Modules planifiés**
   - Support LoRaWAN
   - Base de données locale
   - Interface Bluetooth

2. **Optimisations**
   - Compression des données
   - Cache distribué
   - Optimisation mémoire

3. **Nouvelles fonctionnalités**
   - Support multi-caméras
   - Fusion de données
   - ML embarqué avancé

## Support et ressources

- Documentation API : `/docs/api/`
- Exemples : `/examples/`
- Tests : `/test/`
- Scripts : `/scripts/`

## Notes de version

### v1.1.0 (actuelle)
- Support complet macOS M2
- Amélioration du système de build
- Interface de mise à jour intégrée

### v1.0.2
- Système de build amélioré
- Corrections de bugs

### v1.0.1
- Support initial macOS
- Optimisations