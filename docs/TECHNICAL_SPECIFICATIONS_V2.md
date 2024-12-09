# Spécifications Techniques V2

## Architecture du Projet

### Structure des Dossiers
```
M5stackHuskyLens/
├── src/                    # Code source principal
│   ├── api/               # API REST et serveur web
│   ├── core/              # Composants fondamentaux
│   ├── display/           # Gestion de l'affichage
│   ├── system/            # Gestion système
│   └── utils/             # Utilitaires
├── lib/                    # Bibliothèques externes
├── test/                   # Tests unitaires et d'intégration
├── docs/                   # Documentation
└── scripts/                # Scripts utilitaires
```

### Composants Principaux

#### 1. Interface Utilisateur (DisplayManager)
- Gestion de l'écran LCD M5Stack
- Support du mode jour/nuit
- Affichage des QR codes
- Interface de configuration

#### 2. Système de Configuration (ConfigManager)
- Gestion des paramètres
- Persistance des données
- Configuration WiFi
- États du système

#### 3. Gestionnaire de Démarrage (BootManager)
- Séquence de démarrage
- Vérification du matériel
- Initialisation des composants
- Gestion des erreurs

#### 4. API REST (WebServer)
- Configuration à distance
- Monitoring en temps réel
- Gestion des mises à jour
- Sécurité et authentification

## Spécifications Matérielles

### M5Stack CoreS3
- **Processeur**: ESP32-S3
- **RAM**: 512KB SRAM
- **Flash**: 16MB
- **Écran**: LCD 320x240
- **Connectivité**: WiFi + BLE

### HuskyLens
- **Processeur**: K210
- **Algorithmes**: Vision par ordinateur
- **Interface**: I2C/UART
- **Caméra**: OV2640

## Spécifications Logicielles

### Environnement de Développement
- **Framework**: Arduino
- **Plateforme**: PlatformIO
- **Langage**: C++
- **Version C++**: C++17

### Dépendances Principales
```ini
m5stack/M5CoreS3@^1.0.0
m5stack/M5Unified@^0.2.1
huskylens/HuskyLens@^1.0.0
bblanchon/ArduinoJson@^6.21.5
fastled/FastLED@^3.9.4
plerup/EspSoftwareSerial@^8.2.0
lorol/LittleFS_esp32@^1.0.6
```

## Protocoles et Interfaces

### Communication
- **WiFi**: 2.4GHz, 802.11 b/g/n
- **I2C**: Communication HuskyLens
- **HTTP/HTTPS**: API REST
- **WebSocket**: Données en temps réel

### Stockage
- **EEPROM**: Configuration
- **SPIFFS**: Fichiers système
- **SD Card**: Données utilisateur (optionnel)

## Performance

### Mémoire
- RAM utilisée: ~54KB
- Flash utilisée: ~1.3MB
- PSRAM disponible: 8MB

### Temps de Réponse
- Démarrage: <3s
- Reconnaissance: <100ms
- Rafraîchissement UI: 60fps

## Sécurité

### Authentification
- JWT pour l'API
- Chiffrement des données
- Protection contre les attaques courantes

### Données
- Sauvegarde chiffrée
- Validation des entrées
- Sanitization des données

## Configuration

### Variables d'Environnement
```cpp
// Définies dans Config.h
#define DEBUG_MODE
#define WIFI_TIMEOUT 10000
#define MAX_RETRY_ATTEMPTS 3
```

### Options de Compilation
```ini
build_flags = 
    -DCORE_DEBUG_LEVEL=5
    -DESP32=1
    -DBOARD_HAS_PSRAM
```

## Gestion des Erreurs

### Journalisation
- Niveaux de log: ERROR, WARN, INFO, DEBUG
- Rotation des logs
- Horodatage des événements

### Récupération
- Watchdog matériel
- Safe mode
- Rollback automatique

## Tests

### Tests Unitaires
- Framework: Unity
- Couverture: >80%
- Tests automatisés

### Tests d'Intégration
- Tests système
- Tests de performance
- Tests de charge

## Mise à Jour

### OTA (Over-The-Air)
- Mise à jour progressive
- Vérification d'intégrité
- Rollback automatique

### Version
- Gestion sémantique
- Changelog automatique
- Notes de version

## Limitations Connues

1. **Matérielles**
   - Limite de RAM
   - Consommation CPU
   - Température maximale

2. **Logicielles**
   - Taille du buffer vidéo
   - Nombre de connexions simultanées
   - Taille maximale des fichiers

## Roadmap Technique

### Court Terme (v2.1)
- [ ] Optimisation mémoire
- [ ] Amélioration stabilité WiFi
- [ ] Tests automatisés

### Moyen Terme (v2.2)
- [ ] Nouveaux algorithmes CV
- [ ] Interface web améliorée
- [ ] Support Bluetooth

### Long Terme (v3.0)
- [ ] IA embarquée
- [ ] Cloud integration
- [ ] Multi-device support