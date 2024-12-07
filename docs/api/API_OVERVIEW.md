# Vue d'Ensemble de l'API M5Stack HuskyLens

## Introduction

L'API M5Stack HuskyLens fournit une interface complète pour interagir avec le système de vision. Elle est divisée en plusieurs composants :
- API REST pour le contrôle à distance
- API Vision pour le traitement d'image
- API Machine Learning pour l'IA
- API Matérielle pour le contrôle bas niveau

## Architecture API

```
API M5Stack HuskyLens
├── REST API
│   ├── Vision Control
│   ├── System Control
│   ├── ML Management
│   └── Analytics
├── Vision API
│   ├── Object Detection
│   ├── Face Recognition
│   ├── Color Detection
│   └── Line Tracking
├── ML API
│   ├── Model Management
│   ├── Training
│   ├── Inference
│   └── Analytics
└── Hardware API
    ├── Camera Control
    ├── Display
    ├── I/O
    └── System
```

## REST API

### Points d'Entrée Principaux

#### Vision Control
```http
GET /api/v1/vision/status
POST /api/v1/vision/mode
PUT /api/v1/vision/settings
```

#### System Control
```http
GET /api/v1/system/status
POST /api/v1/system/reboot
PUT /api/v1/system/config
```

#### ML Management
```http
GET /api/v1/ml/models
POST /api/v1/ml/train
DELETE /api/v1/ml/models/{id}
```

#### Analytics
```http
GET /api/v1/analytics/performance
GET /api/v1/analytics/history
POST /api/v1/analytics/export
```

### Exemples d'Utilisation

```bash
# Obtenir le statut du système
curl -X GET http://device-ip/api/v1/system/status

# Changer le mode de vision
curl -X POST http://device-ip/api/v1/vision/mode \
  -H "Content-Type: application/json" \
  -d '{"mode": "face_recognition"}'
```

## Vision API

### Classes Principales

```cpp
class VisionSystem {
public:
    void setMode(VisionMode mode);
    void configure(VisionConfig config);
    DetectionResult detect();
    void startTracking(int objectId);
    void stopTracking();
};
```

### Exemples d'Utilisation

```cpp
// Configuration de la détection d'objets
visionSystem.setMode(VisionMode::OBJECT_DETECTION);
VisionConfig config;
config.sensitivity = 0.8f;
config.minSize = 20;
visionSystem.configure(config);

// Démarrer la détection
auto result = visionSystem.detect();
```

## ML API

### Classes Principales

```cpp
class MLSystem {
public:
    void loadModel(const String& modelPath);
    void train(const TrainingData& data);
    void startInference();
    InferenceResult predict(const InputData& input);
};
```

### Exemples d'Utilisation

```cpp
// Charger un modèle
mlSystem.loadModel("/models/object_detection.tflite");

// Démarrer l'inférence
mlSystem.startInference();

// Prédiction
InputData input;
auto result = mlSystem.predict(input);
```

## Hardware API

### Classes Principales

```cpp
class HardwareSystem {
public:
    void initCamera();
    void setDisplayBrightness(uint8_t level);
    void configureI2C(uint8_t sda, uint8_t scl);
    SystemStatus getStatus();
};
```

### Exemples d'Utilisation

```cpp
// Configuration matérielle
hardwareSystem.configureI2C(2, 1);
hardwareSystem.initCamera();
hardwareSystem.setDisplayBrightness(128);
```

## Gestion des Erreurs

### Codes d'Erreur
- 1000-1999 : Erreurs système
- 2000-2999 : Erreurs vision
- 3000-3999 : Erreurs ML
- 4000-4999 : Erreurs matérielles

### Structure d'Erreur
```cpp
struct Error {
    uint16_t code;
    String message;
    ErrorSeverity severity;
};
```

### Traitement des Erreurs
```cpp
try {
    visionSystem.startDetection();
} catch (VisionError& e) {
    logger.error("Vision error: " + e.message);
    handleError(e);
}
```

## Sécurité

### Authentification
```cpp
// Génération de token
String token = security.generateToken(userId);

// Vérification de token
bool isValid = security.verifyToken(token);
```

### Autorisation
```cpp
// Vérification des droits
if (!security.hasPermission(user, Permission::VISION_CONTROL)) {
    throw AuthorizationError("Insufficient permissions");
}
```

## Bonnes Pratiques

1. **Gestion des Ressources**
   ```cpp
   // Utiliser RAII pour la gestion des ressources
   class VisionSession {
       ~VisionSession() {
           cleanup();
       }
   };
   ```

2. **Validation des Entrées**
   ```cpp
   void setParameter(const String& name, float value) {
       if (!isValidParameter(name)) {
           throw InvalidParameterError(name);
       }
       parameters[name] = value;
   }
   ```

3. **Logging**
   ```cpp
   logger.info("Starting vision detection");
   auto result = visionSystem.detect();
   logger.debug("Detection result: " + result.toString());
   ```

## Dépannage

1. **Vérification Connectivité**
   ```bash
   curl -v http://device-ip/api/v1/system/status
   ```

2. **Test Vision**
   ```cpp
   auto status = visionSystem.selfTest();
   if (!status.isOK()) {
       handleError(status.error);
   }
   ```

3. **Diagnostic ML**
   ```cpp
   MLDiagnostic diag = mlSystem.runDiagnostic();
   logger.info("ML System Status: " + diag.toString());
   ```