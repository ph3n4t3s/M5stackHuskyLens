# README

# Cahier des Charges - Système de Vision par Ordinateur M5Stack-HuskyLens

## 1. Présentation du Projet

### 1.1 Objectif
Développer une interface complète entre le M5Stack Core S3 et le module HuskyLens, permettant d'exploiter toutes les capacités de vision par ordinateur du HuskyLens avec une interface utilisateur intuitive et des fonctionnalités étendues.

### 1.2 Matériel Requis
- M5Stack Core S3
- Module HuskyLens
- Câbles de connexion I2C
- Support de montage (optionnel)

### 1.3 Configuration Matérielle
- Connexion I2C :
  * SDA : Pin 2 (M5Stack Core S3)
  * SCL : Pin 1 (M5Stack Core S3)
  * Alimentation : 5V via M5Stack
  * Masse : GND commun

## 2. Spécifications Fonctionnelles

### 2.1 Modes de Détection
1. **Reconnaissance Faciale**
   - Détection de visages multiples
   - Apprentissage de visages spécifiques
   - Reconnaissance des visages mémorisés

2. **Suivi d'Objets**
   - Détection d'objets en temps réel
   - Suivi de trajectoire
   - Calcul de vitesse de déplacement

3. **Suivi de Lignes**
   - Détection de lignes et intersections
   - Suivi de parcours
   - Détection de virages

4. **Reconnaissance de Couleurs**
   - Détection de couleurs spécifiques
   - Classification par couleur
   - Analyse de zones de couleur

5. **Reconnaissance de Tags**
   - Lecture de tags AprilTag
   - Identification unique
   - Position et orientation

6. **Classification d'Objets**
   - Apprentissage d'objets personnalisés
   - Classification multi-objets
   - Statistiques de reconnaissance

7. **Mode Gestuel**
   - Reconnaissance de mouvements
   - Détection de gestes personnalisés
   - Commandes par geste

8. **Mesure de Distance**
   - Estimation de distance aux objets
   - Calcul de dimensions
   - Zones de proximité

### 2.2 Interface Utilisateur

#### 2.2.1 Modes d'Affichage
1. **Vue Raw Data**
   - Données brutes du capteur
   - Coordonnées et mesures
   - Informations techniques

2. **Vue Processed Info**
   - Informations traitées
   - Statistiques en temps réel
   - Analyses de performance

3. **Interface Graphique**
   - Affichage visuel intuitif
   - Indicateurs de détection
   - Suivi en temps réel

4. **Vue Analytics**
   - Graphiques de performance
   - Historique des détections
   - Analyses statistiques

5. **Vue Split Screen**
   - Affichage multiple simultané
   - Comparaison de modes
   - Monitoring multi-données

6. **Mode Debug**
   - Informations de débogage
   - État du système
   - Logs d'erreurs

#### 2.2.2 Contrôles
- **Bouton A** : Navigation entre modes HuskyLens
- **Bouton B** : Changement de mode d'affichage
- **Bouton C** : Capture/Apprentissage
- **A + C** : Accès au menu de configuration

### 2.3 Fonctionnalités Système

#### 2.3.1 Configuration
- Sensibilité de détection ajustable
- Niveau de zoom paramétrable
- Mode nuit
- Auto-apprentissage
- Retour audio configurable
- Intervalle d'enregistrement

#### 2.3.2 Gestion des Données
- Sauvegarde des configurations
- Historique des détections
- Export de données
- Gestion de la mémoire

#### 2.3.3 Automatisation
- Apprentissage automatique
- Détection de zones
- Alertes configurables
- Enregistrement automatique

## 3. Spécifications Techniques

### 3.1 Architecture Logicielle
1. **Classes Principales**
   - HuskyLensPlus
   - DisplayManager
   - DataProcessor
   - Configuration Manager

2. **Structures de Données**
   - Point
   - DetectionZone
   - HistoricalData
   - SensorData
   - DisplayData

### 3.2 Performance
- Taux de rafraîchissement : 20+ FPS
- Temps de réponse < 100ms
- Gestion mémoire optimisée
- Utilisation CPU optimisée

### 3.3 Contraintes
- Mémoire disponible : 320KB RAM
- Flash : 16MB
- Résolution écran : 320x240
- Protocole I2C : 100kHz standard

## 4. Évolutions Futures Planifiées

### 4.1 Améliorations Prévues
1. Interface réseau pour données distantes
2. Mode multi-caméras
3. Intelligence artificielle embarquée
4. Interface web de configuration

### 4.2 Extensions Possibles
1. Support de modules additionnels
2. Protocoles de communication étendus
3. Modes de détection personnalisés
4. Analyses avancées

## 5. Documentation

### 5.1 Documentation Utilisateur
- Manuel d'installation
- Guide d'utilisation
- Tutoriels de configuration
- FAQ et dépannage

### 5.2 Documentation Technique
- Architecture système
- Diagrammes de classes
- Protocoles de communication
- Guide de développement

## 6. Tests et Validation

### 6.1 Tests Unitaires
- Fonctions individuelles
- Gestion des erreurs
- Performance des algorithmes

### 6.2 Tests d'Intégration
- Communication I2C
- Interface utilisateur
- Gestion de la mémoire

### 6.3 Tests de Performance
- Temps de réponse
- Précision de détection
- Stabilité système

Ce cahier des charges servira de référence pour la poursuite du développement et permettra d'assurer une implémentation cohérente et complète du système.


# Résumé de la situation actuelle

1. Configuration du projet :

```ini
[env:m5stack-cores3]
platform = espressif32
board = m5stack-cores3
framework = arduino
monitor_speed = 115200
upload_speed = 1500000
lib_deps = 
    m5stack/M5CoreS3
    Wire
    SoftwareSerial
    huskylens/HuskyLens@^1.0.0
    moononournation/GFX Library for Arduino@^1.5.0
    m5stack/M5Unified@^0.2.1
build_flags = 
    -DCORE_DEBUG_LEVEL=0
    -DBOARD_HAS_PSRAM
    -mfix-esp32-psram-cache-issue
```

2. Structure du projet :

```
M5Stack_HuskyLens_Project/
├── platformio.ini
├── src/
│   ├── main.cpp
│   ├── Config.h
│   ├── HuskyLensPlus.h
│   ├── HuskyLensPlus.cpp
│   ├── DisplayManager.h
│   ├── DisplayManager.cpp
│   ├── DataProcessor.h
│   └── DataProcessor.cpp
├── include/
├── lib/
└── README.md
```

3. État actuel :
- Le projet vise à créer une interface entre M5Stack Core S3 et HuskyLens
- Principales fonctionnalités implémentées :
  * Reconnaissance faciale
  * Suivi d'objets
  * Suivi de lignes
  * Reconnaissance de couleurs
  * Reconnaissance de tags
  * Mesure de distance
  * Interface graphique personnalisable

4. Problèmes résolus :
- Erreurs de compilation liées aux types min/max
- Fonction processStandardData manquante
- Problèmes de compatibilité avec la bibliothèque HuskyLens

5. Points à développer :
- Amélioration de l'interface utilisateur
- Ajout de nouvelles fonctionnalités de détection
- Optimisation des performances
- Meilleure gestion de la mémoire

6. Connexions matérielles :
- M5Stack Core S3 connecté au HuskyLens via I2C
- SDA sur Pin 2
- SCL sur Pin 1

7. Commandes de base :
- Bouton A : Change le mode HuskyLens
- Bouton B : Change le mode d'affichage
- Bouton C : Capture/Apprentissage
- A + C : Menu de configuration

Pour la prochaine session, nous pourrons :
1. Améliorer la stabilité du code
2. Ajouter de nouvelles fonctionnalités
3. Optimiser l'interface utilisateur
4. Implémenter la sauvegarde des configurations
5. Ajouter des modes de détection personnalisés

## Erreurs de compilation

```
Processing m5stack-cores3 (platform: espressif32; board: m5stack-cores3; framework: arduino)
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Verbose mode can be enabled via `-v, --verbose` option
CONFIGURATION: https://docs.platformio.org/page/boards/espressif32/m5stack-cores3.html
PLATFORM: Espressif 32 (6.4.0+sha.f6ec392) > M5Stack CoreS3
HARDWARE: ESP32S3 240MHz, 320KB RAM, 16MB Flash
DEBUG: Current (cmsis-dap) External (cmsis-dap, esp-bridge, esp-builtin, esp-prog, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa)
PACKAGES: 
 - framework-arduinoespressif32 @ 3.20011.230801 (2.0.11) 
 - tool-esptoolpy @ 1.40501.0 (4.5.1) 
 - toolchain-riscv32-esp @ 8.4.0+2021r2-patch5 
 - toolchain-xtensa-esp32s3 @ 8.4.0+2021r2-patch5
LDF: Library Dependency Finder -> https://bit.ly/configure-pio-ldf
LDF Modes: Finder ~ chain, Compatibility ~ soft
Found 39 compatible libraries
Scanning dependencies...
Dependency Graph
|-- M5CoreS3 @ 1.0.0
|-- Wire @ 2.0.0
|-- SoftwareSerial @ 1.0.0
|-- HuskyLens @ 1.0.0
|-- GFX Library for Arduino @ 1.5.0
|-- M5Unified @ 0.2.1
Building in release mode
Compiling .pio/build/m5stack-cores3/src/DataProcessor.cpp.o
Compiling .pio/build/m5stack-cores3/src/HuskyLensPlus.cpp.o
Compiling .pio/build/m5stack-cores3/src/main.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/utility/lgfx_qoi.c.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/utility/lgfx_qrcode.c.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/utility/lgfx_tjpgd.c.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/LGFXBase.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/LGFX_Button.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/LGFX_Sprite.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/lgfx_fonts.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/misc/DividedFrameBuffer.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/misc/SpriteBuffer.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/misc/common_function.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/misc/pixelcopy.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/panel/Panel_Device.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/panel/Panel_FrameBufferBase.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/panel/Panel_GDEW0154D67.cpp.o
Compiling .pio/build/m5stack-cores3/lib69b/M5GFX/lgfx/v1/panel/Panel_GDEW0154M09.cpp.o
src/DataProcessor.cpp: In member function 'void DataProcessor::process(const SensorData&)':
src/DataProcessor.cpp:28:13: error: 'processStandardData' was not declared in this scope
             processStandardData(data);
             ^~~~~~~~~~~~~~~~~~~
src/DataProcessor.cpp:28:13: note: suggested alternative: 'processGestureData'
             processStandardData(data);
             ^~~~~~~~~~~~~~~~~~~
             processGestureData
src/DataProcessor.cpp: At global scope:
src/DataProcessor.cpp:116:6: error: no declaration matches 'void DataProcessor::processStandardData(const SensorData&)'
 void DataProcessor::processStandardData(const SensorData& data) {
      ^~~~~~~~~~~~~
src/DataProcessor.cpp:116:6: note: no functions named 'void DataProcessor::processStandardData(const SensorData&)'
In file included from src/DataProcessor.cpp:1:
src/DataProcessor.h:7:7: note: 'class DataProcessor' defined here
 class DataProcessor {
       ^~~~~~~~~~~~~
In file included from src/HuskyLensPlus.h:5,
                 from src/HuskyLensPlus.cpp:1:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h: In member function 'bool HUSKYLENS::processReturn()':
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:171:90: error: no matching function for call to 'max(int16_t&, int)'
         protocolPtr = (Protocol_t*) realloc(protocolPtr, max(protocolInfo.protocolSize, 1) * sizeof(Protocol_t));
                                                                                          ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:62,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from src/HuskyLensPlus.h:3,
                 from src/HuskyLensPlus.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3468:5: note: candidate: 'template<class _Tp, class _Compare> _Tp std::max(std::initializer_list<_Tp>, _Compare)'
     max(initializer_list<_Tp> __l, _Compare __comp)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3468:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/HuskyLensPlus.cpp:1:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:171:90: note:   mismatched types 'std::initializer_list<_Tp>' and 'short int'
         protocolPtr = (Protocol_t*) realloc(protocolPtr, max(protocolInfo.protocolSize, 1) * sizeof(Protocol_t));
                                                                                          ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:62,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from src/HuskyLensPlus.h:3,
                 from src/HuskyLensPlus.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3462:5: note: candidate: 'template<class _Tp> _Tp std::max(std::initializer_list<_Tp>)'
     max(initializer_list<_Tp> __l)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3462:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/HuskyLensPlus.cpp:1:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:171:90: note:   mismatched types 'std::initializer_list<_Tp>' and 'short int'
         protocolPtr = (Protocol_t*) realloc(protocolPtr, max(protocolInfo.protocolSize, 1) * sizeof(Protocol_t));
                                                                                          ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:61,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from src/HuskyLensPlus.h:3,
                 from src/HuskyLensPlus.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:265:5: note: candidate: 'template<class _Tp, class _Compare> const _Tp& std::max(const _Tp&, const _Tp&, _Compare)'
     max(const _Tp& __a, const _Tp& __b, _Compare __comp)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:265:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/HuskyLensPlus.cpp:1:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:171:90: note:   deduced conflicting types for parameter 'const _Tp' ('short int' and 'int')
         protocolPtr = (Protocol_t*) realloc(protocolPtr, max(protocolInfo.protocolSize, 1) * sizeof(Protocol_t));
                                                                                          ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:61,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from src/HuskyLensPlus.h:3,
                 from src/HuskyLensPlus.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:219:5: note: candidate: 'template<class _Tp> const _Tp& std::max(const _Tp&, const _Tp&)'
     max(const _Tp& __a, const _Tp& __b)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:219:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/HuskyLensPlus.cpp:1:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:171:90: note:   deduced conflicting types for parameter 'const _Tp' ('short int' and 'int')
         protocolPtr = (Protocol_t*) realloc(protocolPtr, max(protocolInfo.protocolSize, 1) * sizeof(Protocol_t));
                                                                                          ^
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h: In member function 'int HUSKYLENS::available()':
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:297:48: error: no matching function for call to 'min(int16_t&, int&)'
         currentIndex = min(currentIndex, result);
                                                ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:62,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from src/HuskyLensPlus.h:3,
                 from src/HuskyLensPlus.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3456:5: note: candidate: 'template<class _Tp, class _Compare> _Tp std::min(std::initializer_list<_Tp>, _Compare)'
     min(initializer_list<_Tp> __l, _Compare __comp)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3456:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/HuskyLensPlus.cpp:1:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:297:48: note:   mismatched types 'std::initializer_list<_Tp>' and 'short int'
         currentIndex = min(currentIndex, result);
                                                ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:62,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from src/HuskyLensPlus.h:3,
                 from src/HuskyLensPlus.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3450:5: note: candidate: 'template<class _Tp> _Tp std::min(std::initializer_list<_Tp>)'
     min(initializer_list<_Tp> __l)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3450:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/HuskyLensPlus.cpp:1:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:297:48: note:   mismatched types 'std::initializer_list<_Tp>' and 'short int'
         currentIndex = min(currentIndex, result);
                                                ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:61,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from src/HuskyLensPlus.h:3,
                 from src/HuskyLensPlus.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:243:5: note: candidate: 'template<class _Tp, class _Compare> const _Tp& std::min(const _Tp&, const _Tp&, _Compare)'
     min(const _Tp& __a, const _Tp& __b, _Compare __comp)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:243:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/HuskyLensPlus.cpp:1:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:297:48: note:   deduced conflicting types for parameter 'const _Tp' ('short int' and 'int')
         currentIndex = min(currentIndex, result);
                                                ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:61,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from src/HuskyLensPlus.h:3,
                 from src/HuskyLensPlus.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:195:5: note: candidate: 'template<class _Tp> const _Tp& std::min(const _Tp&, const _Tp&)'
     min(const _Tp& __a, const _Tp& __b)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:195:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/HuskyLensPlus.cpp:1:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:297:48: note:   deduced conflicting types for parameter 'const _Tp' ('short int' and 'int')
         currentIndex = min(currentIndex, result);
                                                ^
*** [.pio/build/m5stack-cores3/src/DataProcessor.cpp.o] Error 1
src/HuskyLensPlus.cpp: In member function 'void HuskyLensPlus::configureMode(HuskyMode)':
src/HuskyLensPlus.cpp:79:38: error: 'ALGORITHM_QR_CODE_RECOGNITION' was not declared in this scope
             huskyLens.writeAlgorithm(ALGORITHM_QR_CODE_RECOGNITION);
                                      ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
src/HuskyLensPlus.cpp:79:38: note: suggested alternative: 'ALGORITHM_FACE_RECOGNITION'
             huskyLens.writeAlgorithm(ALGORITHM_QR_CODE_RECOGNITION);
                                      ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                                      ALGORITHM_FACE_RECOGNITION
src/HuskyLensPlus.cpp: In member function 'void HuskyLensPlus::learn(int)':
src/HuskyLensPlus.cpp:95:15: error: 'class HUSKYLENS' has no member named 'learn'
     huskyLens.learn(id);
               ^~~~~
src/HuskyLensPlus.cpp: In member function 'void HuskyLensPlus::forget()':
src/HuskyLensPlus.cpp:99:15: error: 'class HUSKYLENS' has no member named 'forget'; did you mean 'get'?
     huskyLens.forget();
               ^~~~~~
               get
src/HuskyLensPlus.cpp: In member function 'void HuskyLensPlus::saveModel()':
src/HuskyLensPlus.cpp:103:15: error: 'class HUSKYLENS' has no member named 'saveModelToTFCard'
     huskyLens.saveModelToTFCard();
               ^~~~~~~~~~~~~~~~~
*** [.pio/build/m5stack-cores3/src/HuskyLensPlus.cpp.o] Error 1
In file included from src/HuskyLensPlus.h:5,
                 from src/main.cpp:3:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h: In member function 'bool HUSKYLENS::processReturn()':
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:171:90: error: no matching function for call to 'max(int16_t&, int)'
         protocolPtr = (Protocol_t*) realloc(protocolPtr, max(protocolInfo.protocolSize, 1) * sizeof(Protocol_t));
                                                                                          ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:62,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/../../misc/DataWrapper.hpp:21,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/common.hpp:20,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../common.hpp:22,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/Bus_SPI.hpp:57,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/device.hpp:48,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/M5GFX.h:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.hpp:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.h:5,
                 from .pio/libdeps/m5stack-cores3/M5CoreS3/src/M5CoreS3.h:4,
                 from src/main.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3468:5: note: candidate: 'template<class _Tp, class _Compare> _Tp std::max(std::initializer_list<_Tp>, _Compare)'
     max(initializer_list<_Tp> __l, _Compare __comp)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3468:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/main.cpp:3:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:171:90: note:   mismatched types 'std::initializer_list<_Tp>' and 'short int'
         protocolPtr = (Protocol_t*) realloc(protocolPtr, max(protocolInfo.protocolSize, 1) * sizeof(Protocol_t));
                                                                                          ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:62,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/../../misc/DataWrapper.hpp:21,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/common.hpp:20,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../common.hpp:22,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/Bus_SPI.hpp:57,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/device.hpp:48,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/M5GFX.h:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.hpp:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.h:5,
                 from .pio/libdeps/m5stack-cores3/M5CoreS3/src/M5CoreS3.h:4,
                 from src/main.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3462:5: note: candidate: 'template<class _Tp> _Tp std::max(std::initializer_list<_Tp>)'
     max(initializer_list<_Tp> __l)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3462:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/main.cpp:3:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:171:90: note:   mismatched types 'std::initializer_list<_Tp>' and 'short int'
         protocolPtr = (Protocol_t*) realloc(protocolPtr, max(protocolInfo.protocolSize, 1) * sizeof(Protocol_t));
                                                                                          ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:61,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/../../misc/DataWrapper.hpp:21,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/common.hpp:20,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../common.hpp:22,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/Bus_SPI.hpp:57,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/device.hpp:48,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/M5GFX.h:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.hpp:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.h:5,
                 from .pio/libdeps/m5stack-cores3/M5CoreS3/src/M5CoreS3.h:4,
                 from src/main.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:265:5: note: candidate: 'template<class _Tp, class _Compare> const _Tp& std::max(const _Tp&, const _Tp&, _Compare)'
     max(const _Tp& __a, const _Tp& __b, _Compare __comp)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:265:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/main.cpp:3:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:171:90: note:   deduced conflicting types for parameter 'const _Tp' ('short int' and 'int')
         protocolPtr = (Protocol_t*) realloc(protocolPtr, max(protocolInfo.protocolSize, 1) * sizeof(Protocol_t));
                                                                                          ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:61,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/../../misc/DataWrapper.hpp:21,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/common.hpp:20,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../common.hpp:22,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/Bus_SPI.hpp:57,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/device.hpp:48,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/M5GFX.h:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.hpp:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.h:5,
                 from .pio/libdeps/m5stack-cores3/M5CoreS3/src/M5CoreS3.h:4,
                 from src/main.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:219:5: note: candidate: 'template<class _Tp> const _Tp& std::max(const _Tp&, const _Tp&)'
     max(const _Tp& __a, const _Tp& __b)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:219:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/main.cpp:3:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:171:90: note:   deduced conflicting types for parameter 'const _Tp' ('short int' and 'int')
         protocolPtr = (Protocol_t*) realloc(protocolPtr, max(protocolInfo.protocolSize, 1) * sizeof(Protocol_t));
                                                                                          ^
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h: In member function 'int HUSKYLENS::available()':
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:297:48: error: no matching function for call to 'min(int16_t&, int&)'
         currentIndex = min(currentIndex, result);
                                                ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:62,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/../../misc/DataWrapper.hpp:21,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/common.hpp:20,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../common.hpp:22,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/Bus_SPI.hpp:57,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/device.hpp:48,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/M5GFX.h:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.hpp:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.h:5,
                 from .pio/libdeps/m5stack-cores3/M5CoreS3/src/M5CoreS3.h:4,
                 from src/main.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3456:5: note: candidate: 'template<class _Tp, class _Compare> _Tp std::min(std::initializer_list<_Tp>, _Compare)'
     min(initializer_list<_Tp> __l, _Compare __comp)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3456:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/main.cpp:3:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:297:48: note:   mismatched types 'std::initializer_list<_Tp>' and 'short int'
         currentIndex = min(currentIndex, result);
                                                ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:62,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/../../misc/DataWrapper.hpp:21,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/common.hpp:20,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../common.hpp:22,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/Bus_SPI.hpp:57,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/device.hpp:48,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/M5GFX.h:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.hpp:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.h:5,
                 from .pio/libdeps/m5stack-cores3/M5CoreS3/src/M5CoreS3.h:4,
                 from src/main.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3450:5: note: candidate: 'template<class _Tp> _Tp std::min(std::initializer_list<_Tp>)'
     min(initializer_list<_Tp> __l)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algo.h:3450:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/main.cpp:3:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:297:48: note:   mismatched types 'std::initializer_list<_Tp>' and 'short int'
         currentIndex = min(currentIndex, result);
                                                ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:61,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/../../misc/DataWrapper.hpp:21,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/common.hpp:20,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../common.hpp:22,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/Bus_SPI.hpp:57,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/device.hpp:48,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/M5GFX.h:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.hpp:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.h:5,
                 from .pio/libdeps/m5stack-cores3/M5CoreS3/src/M5CoreS3.h:4,
                 from src/main.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:243:5: note: candidate: 'template<class _Tp, class _Compare> const _Tp& std::min(const _Tp&, const _Tp&, _Compare)'
     min(const _Tp& __a, const _Tp& __b, _Compare __comp)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:243:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/main.cpp:3:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:297:48: note:   deduced conflicting types for parameter 'const _Tp' ('short int' and 'int')
         currentIndex = min(currentIndex, result);
                                                ^
In file included from /Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/algorithm:61,
                 from /Users/stef/.platformio/packages/framework-arduinoespressif32/cores/esp32/Arduino.h:172,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/../../misc/DataWrapper.hpp:21,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../esp32/common.hpp:20,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/../common.hpp:22,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/esp32/Bus_SPI.hpp:57,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/lgfx/v1/platforms/device.hpp:48,
                 from .pio/libdeps/m5stack-cores3/M5GFX/src/M5GFX.h:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.hpp:19,
                 from .pio/libdeps/m5stack-cores3/M5Unified/src/M5Unified.h:5,
                 from .pio/libdeps/m5stack-cores3/M5CoreS3/src/M5CoreS3.h:4,
                 from src/main.cpp:1:
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:195:5: note: candidate: 'template<class _Tp> const _Tp& std::min(const _Tp&, const _Tp&)'
     min(const _Tp& __a, const _Tp& __b)
     ^~~
/Users/stef/.platformio/packages/toolchain-xtensa-esp32s3/xtensa-esp32s3-elf/include/c++/8.4.0/bits/stl_algobase.h:195:5: note:   template argument deduction/substitution failed:
In file included from src/HuskyLensPlus.h:5,
                 from src/main.cpp:3:
.pio/libdeps/m5stack-cores3/HuskyLens/HUSKYLENS.h:297:48: note:   deduced conflicting types for parameter 'const _Tp' ('short int' and 'int')
         currentIndex = min(currentIndex, result);
                                                ^
*** [.pio/build/m5stack-cores3/src/main.cpp.o] Error 1
======================================================================= [FAILED] Took 8.75 seconds =======================================================================

 *  Arrêt du processus de terminal "platformio 'run'". Code de sortie : 1. 
 *  Le terminal sera réutilisé par les tâches, appuyez sur une touche pour le fermer. 
```