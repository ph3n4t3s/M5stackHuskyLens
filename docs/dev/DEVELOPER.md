# Guide du Développeur M5Stack HuskyLens

## Vue d'Ensemble

Ce guide fournit les informations nécessaires pour développer et contribuer au projet M5Stack HuskyLens.

## Table des Matières

- [Environnement de Développement](#environnement-de-développement)
- [Architecture](#architecture)
- [Guidelines de Code](#guidelines-de-code)
- [Workflow de Développement](#workflow-de-développement)
- [Tests](#tests)
- [Performance](#performance)
- [Déploiement](#déploiement)

## Environnement de Développement

### Configuration IDE
1. **VSCode + PlatformIO**
   ```bash
   # Installation PlatformIO
   pip install -U platformio
   
   # Extensions VSCode requises
   code --install-extension platformio.platformio-ide
   ```

2. **Configuration Projet**
   ```ini
   [env:esp32-s3-dev]
   platform = espressif32
   board = esp32-s3-devkitc-1
   framework = arduino
   monitor_speed = 115200
   ```

### Dépendances
```ini
lib_deps = 
    m5stack/M5CoreS3@^1.0.0
    m5stack/M5Unified@^0.1.4
    huskylens/HuskyLens@^1.0.0
    bblanchon/ArduinoJson@^6.21.3
    fastled/FastLED@^3.5.0
    ESPAsyncWebSrv
```

## Architecture

### Structure du Projet
```
M5stackHuskyLens/
├── src/
│   ├── core/           # Composants principaux
│   ├── vision/         # Traitement vision
│   ├── ml/            # Machine Learning
│   ├── api/           # API REST
│   └── utils/         # Utilitaires
├── include/           # Headers
├── lib/              # Bibliothèques
├── test/             # Tests
└── docs/             # Documentation
```

### Composants Principaux

#### Core System
```cpp
class CoreSystem {
    VisionSystem vision;
    MLSystem ml;
    NetworkManager network;
    ConfigManager config;
public:
    void initialize();
    void update();
};
```

#### Vision System
```cpp
class VisionSystem {
    HuskyLens huskyLens;
    ImageProcessor processor;
public:
    void processFrame();
    DetectionResult detect();
};
```

#### ML System
```cpp
class MLSystem {
    TFLiteInterpreter interpreter;
    ModelManager models;
public:
    void loadModel(const String& path);
    void predict(const InputData& data);
};
```

## Guidelines de Code

### Style de Code
```cpp
// Classes : PascalCase
class ImageProcessor {
    // Membres privés : camelCase avec underscore
    int _frameCount;
    
public:
    // Méthodes publiques : camelCase
    void processFrame() {
        // Variables locales : camelCase
        int frameWidth = getWidth();
    }
};
```

### Documentation
```cpp
/**
 * @brief Traite une image pour la détection d'objets
 * @param image Image source à traiter
 * @return Résultat de la détection
 * @throws VisionError si l'image est invalide
 */
DetectionResult processImage(const Image& image);
```

### Gestion des Erreurs
```cpp
try {
    vision.processFrame();
} catch (const VisionError& e) {
    logger.error("Vision error: %s", e.what());
    handleError(e);
} catch (const std::exception& e) {
    logger.error("Unexpected error: %s", e.what());
    systemReset();
}
```

## Workflow de Développement

### Git Workflow
```bash
# Créer une branche feature
git checkout -b feature/new-vision-mode

# Développement
git add .
git commit -m "feat: ajout nouveau mode vision"

# Push et PR
git push origin feature/new-vision-mode
```

### Conventions de Commit
```
feat: nouvelle fonctionnalité
fix: correction de bug
docs: modification documentation
style: formatage, pas de changement de code
refactor: refactorisation du code
test: ajout ou modification de tests
```

## Tests

### Tests Unitaires
```cpp
TEST_CASE("Vision Detection") {
    VisionSystem vision;
    
    SECTION("Object Detection") {
        auto result = vision.detectObjects();
        REQUIRE(result.objects.size() > 0);
    }
}
```

### Tests d'Intégration
```cpp
TEST_CASE("System Integration") {
    CoreSystem system;
    system.initialize();
    
    SECTION("Vision + ML") {
        auto result = system.processFrame();
        CHECK(result.isValid());
    }
}
```

### Tests de Performance
```cpp
BENCHMARK("Vision Processing") {
    VisionSystem vision;
    return vision.processFrame();
}
```

## Performance

### Optimisation Mémoire
```cpp
void optimizeMemory() {
    // Utiliser StaticJsonDocument pour les petits JSON
    StaticJsonDocument<200> doc;
    
    // Arena allocator pour les allocations temporaires
    ScopedArena arena(1024);
}
```

### Optimisation CPU
```cpp
void optimizeCPU() {
    // Utiliser IRAM_ATTR pour le code critique
    void IRAM_ATTR processFrameISR() {
        // Code temps-réel
    }
    
    // Multitâche avec FreeRTOS
    xTaskCreatePinnedToCore(
        visionTask,
        "Vision",
        8192,
        NULL,
        1,
        NULL,
        VISION_CORE
    );
}
```

## Déploiement

### Build Production
```bash
# Clean build
pio run -t clean

# Build production
pio run -e production
```

### OTA Updates
```cpp
void setupOTA() {
    AsyncElegantOTA.begin(&server);
    server.begin();
}
```

### Monitoring
```cpp
void setupMonitoring() {
    // Télémétrie système
    metrics.addGauge("vision_fps");
    metrics.addCounter("detections");
    
    // Endpoint monitoring
    server.on("/metrics", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", metrics.toString());
    });
}
```

## Ressources

### Documentation Externe
- [ESP32-S3 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32-s3_technical_reference_manual_en.pdf)
- [HuskyLens Wiki](https://wiki.dfrobot.com/HUSKYLENS)
- [M5Stack Core S3 Docs](https://docs.m5stack.com/en/core/CoreS3)

### Outils Utiles
- [PlatformIO Debug Tools](https://docs.platformio.org/en/latest/plus/debugging.html)
- [ESP Memory Analyzer](https://github.com/espressif/esp-mem)
- [Visual Memory Profiler](https://github.com/espressif/esp-visual-profiler)

## Support

### Canaux de Communication
- GitHub Issues pour les bugs
- Discussions GitHub pour les questions
- Wiki pour la documentation communautaire
- Discord pour le chat temps réel

### Debug
```cpp
// Macro de debug conditionnelle
#ifdef DEBUG
    #define DEBUG_PRINT(x) Serial.println(x)
#else
    #define DEBUG_PRINT(x)
#endif

// Utilisation
DEBUG_PRINT("Vision state: " + state.toString());
```