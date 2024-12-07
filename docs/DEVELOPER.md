# Guide Développeur

## Architecture du Système

### Vue d'ensemble
Le système est construit autour de plusieurs composants principaux qui interagissent entre eux :

```
                   ┌─────────────┐
                   │  HuskyLens  │
                   └─────┬───────┘
                         │
                   ┌─────▼───────┐
┌──────────────┐   │            │   ┌────────────┐
│ MLSystem     │◄──┤ Processor  ├──►│ ImageProc  │
└──────────────┘   │            │   └────────────┘
                   └─────┬───────┘
                         │
     ┌──────────────────┼──────────────────┐
     │                  │                   │
┌────▼─────┐     ┌─────▼───────┐    ┌─────▼─────┐
│ Display  │     │ Automation  │    │  Logger   │
└──────────┘     └─────────────┘    └───────────┘
```

### Composants Principaux
1. HuskyLensPlus : Interface avec le capteur
2. Processor : Traitement central des données
3. ImageProcessor : Traitement d'image
4. MLSystem : Intelligence artificielle
5. AutomationSystem : Règles et actions
6. Display : Interface utilisateur
7. Logger : Journalisation

## Contribution au Projet

### Configuration de l'Environnement
1. Installation des outils :
   ```bash
   # Installation de PlatformIO
   pip install platformio
   
   # Configuration du projet
   pio project init --board m5stack-cores3
   ```

2. Configuration de VSCode :
   - Installer l'extension PlatformIO
   - Ouvrir le projet
   - Configurer les paramètres de build

### Standards de Code

#### Style de Code
```cpp
// Classes en PascalCase
class ImageProcessor {
public:
    // Méthodes en camelCase
    void processImage();
    
private:
    // Variables membres avec m_prefix
    int m_width;
    float m_threshold;
};

// Constantes en UPPER_CASE
const int MAX_OBJECTS = 10;

// Enums en PascalCase
enum class DisplayMode {
    RawData,
    ProcessedInfo,
    Analytics
};
```

#### Organisation des Fichiers
```
src/
├── core/           # Composants principaux
│   ├── HuskyLensPlus.*
│   └── Processor.*
├── vision/         # Traitement d'image
│   ├── ImageProcessor.*
│   └── Filters.*
├── ml/            # Machine Learning
│   ├── MLSystem.*
│   └── Models.*
├── automation/     # Automatisation
│   ├── AutomationSystem.*
│   └── Rules.*
├── ui/            # Interface utilisateur
│   ├── Display.*
│   └── Menu.*
└── utils/         # Utilitaires
    ├── Logger.*
    └── Config.*
```

### Tests

#### Tests Unitaires
```cpp
void testImageProcessor() {
    ImageProcessor processor;
    
    // Test de base
    ASSERT(processor.begin());
    
    // Test des filtres
    SensorData data;
    processor.processImage(data);
    ASSERT(data.points.size() > 0);
    
    // Test vision nocturne
    processor.enableNightMode(true);
    ASSERT(processor.isNightModeEnabled());
}
```

#### Tests d'Intégration
```cpp
void testSystemIntegration() {
    HuskyLensPlus husky;
    ImageProcessor imgProc;
    MLSystem ml;
    
    // Test flux complet
    husky.begin();
    SensorData data = husky.getData();
    imgProc.processImage(data);
    auto prediction = ml.predict("model1", data);
    
    ASSERT(prediction.confidence > 0.8f);
}
```

### Optimisation

#### Mémoire
```cpp
// Utiliser des références pour éviter les copies
void processData(const SensorData& data) {
    // Traitement sans copie
}

// Réutiliser les buffers
class ImageProcessor {
private:
    std::vector<uint8_t> m_buffer;  // Buffer réutilisable
    
public:
    void processImage() {
        m_buffer.clear();  // Réutiliser au lieu de réallouer
    }
};
```

#### Performance
```cpp
// Optimisation des boucles
void processPixels() {
    const uint32_t* data = (uint32_t*)imageData;
    const int size = width * height / 4;
    
    // Traiter 4 pixels à la fois
    for (int i = 0; i < size; i++) {
        process4Pixels(data[i]);
    }
}

// Utiliser PSRAM pour les gros tableaux
ps_malloc(size);
```

### Débogage

#### Logging
```cpp
// Niveaux de log
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

// Utilisation
logger.log(LogLevel::DEBUG, "Processing image...");
logger.log(LogLevel::ERROR, "Failed to connect: %s", error.c_str());
```

#### Visualisation
```cpp
// Afficher les données de debug
void displayDebugInfo() {
    M5.Lcd.printf("FPS: %d\n", getFPS());
    M5.Lcd.printf("Memory: %d KB\n", ESP.getFreeHeap() / 1024);
    M5.Lcd.printf("Objects: %d\n", objectCount);
}
```

### Publication

#### Versioning
```bash
# Création d'une nouvelle version
git tag -a v1.1.0 -m "Version 1.1.0"
git push origin v1.1.0

# Notes de version
cat > CHANGELOG.md << EOL
# v1.1.0
## Nouveautés
- Vision nocturne améliorée
- Support des gestes complexes

## Corrections
- Stabilité WiFi améliorée
- Optimisation mémoire
EOL
```

#### Documentation
```cpp
/**
 * @brief Traite une image en appliquant les filtres configurés
 * @param data Données du capteur à traiter
 * @param enhancement Paramètres d'amélioration
 * @return true si le traitement est réussi
 */
bool processImage(SensorData& data, const ImageEnhancement& enhancement);
```

## Ressources

### Outils de Développement
- PlatformIO CLI
- ESP-IDF Monitor
- M5Stack Tools
- GDB pour ESP32

### Documentation Externe
- [M5Stack Core S3 Documentation](https://docs.m5stack.com/)
- [HuskyLens Documentation](https://wiki.dfrobot.com/HUSKYLENS)
- [ESP32-S3 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32-s3_technical_reference_manual_en.pdf)

### Communauté
- Forum M5Stack
- Discord ESP32
- GitHub Issues

## Workflow de Développement

### Git
```bash
# Création de branche
git checkout -b feature/night-vision

# Commits atomiques
git commit -m "feat: ajout mode vision nocturne"
git commit -m "fix: correction contraste nuit"

# Pull Request
git push origin feature/night-vision
```

### CI/CD
```yaml
# .github/workflows/build.yml
name: Build
on: [push, pull_request]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Build
        run: |
          pip install platformio
          pio run
```