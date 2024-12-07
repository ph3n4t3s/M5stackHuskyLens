# M5Stack HuskyLens - Système de Vision par Ordinateur Avancé

<div align="center">

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![PlatformIO](https://img.shields.io/badge/platformio-esp32s3-orange.svg)](https://platformio.org/)
[![M5Stack](https://img.shields.io/badge/m5stack-cores3-red.svg)](https://shop.m5stack.com/)
[![Version](https://img.shields.io/badge/version-1.0.0-green.svg)](https://github.com/ph3n4t3s/M5stackHuskyLens/releases)

*Un système de vision par ordinateur avancé combinant M5Stack Core S3 et HuskyLens, enrichi d'IA et de capacités de traitement en temps réel.*

[Features](#features) • [Installation](#installation) • [Documentation](#documentation) • [Utilisation](#utilisation) • [Contribution](#contribution) 

</div>

## 📋 Table des Matières
- [Aperçu](#aperçu)
- [Features](#features)
- [Architecture](#architecture)
- [Prérequis](#prérequis)
- [Installation](#installation)
- [Configuration](#configuration)
- [Utilisation](#utilisation)
- [Documentation](#documentation)
- [Performance](#performance)
- [Contribution](#contribution)
- [Licence](#licence)
- [Contact](#contact)

## 🔍 Aperçu
Le projet M5Stack HuskyLens est une solution complète de vision par ordinateur qui combine la puissance du M5Stack Core S3 avec les capacités avancées du module HuskyLens. Il offre une large gamme de fonctionnalités de vision par ordinateur, d'intelligence artificielle et de traitement d'image en temps réel, le tout dans un package compact et facile à utiliser.

### Caractéristiques Principales
- Vision par ordinateur avancée avec apprentissage en temps réel
- Interface utilisateur intuitive sur écran LCD
- Configuration et monitoring via interface web
- Traitement d'image optimisé pour ESP32-S3
- Support multi-protocoles (I2C, WiFi, Bluetooth)
- Système d'automatisation flexible

## 🚀 Features

### Vision et Reconnaissance
- Reconnaissance faciale multi-personnes
- Suivi d'objets en temps réel
- Détection de lignes et intersections
- Reconnaissance de couleurs et formes
- Détection de tags et QR codes
- Vision nocturne adaptative
- Stabilisation d'image

### Intelligence Artificielle
- TensorFlow Lite pour inférence
- Classification d'objets personnalisée
- Reconnaissance de gestes
- Détection de motifs
- Analyse comportementale
- Apprentissage incrémental

### Traitement d'Image
- Amélioration automatique de qualité
- Filtres configurables en temps réel
- Réduction du bruit intelligente
- Correction lumière adaptative
- Mode vision nocturne
- Stabilisation vidéo

### Automatisation et Analytics
- Système de règles personnalisables
- Actions conditionnelles
- Planification temporelle
- Analyse statistique en temps réel
- Historique des détections
- Export de données

## 🏗 Architecture
```
M5stackHuskyLens/
├── src/                    # Code source
│   ├── main.cpp           # Point d'entrée
│   ├── core/              # Composants principaux
│   ├── vision/            # Traitement vision
│   ├── ml/                # Machine Learning
│   ├── api/               # API REST
│   └── utils/             # Utilitaires
├── include/               # Headers
├── lib/                   # Bibliothèques
├── data/                  # Ressources web
├── docs/                  # Documentation
└── test/                  # Tests unitaires
```

## 📝 Prérequis
- [M5Stack Core S3](https://shop.m5stack.com/products/m5stack-cores3-esp32s3-lotdevelopment-kit)
- [Module HuskyLens](https://wiki.dfrobot.com/HUSKYLENS_V1.0_SKU_SEN0305_SEN0336)
- [PlatformIO IDE](https://platformio.org/platformio-ide)
- Câbles de connexion I2C
- Micro SD Card (optionnelle)

### Spécifications Matérielles
| Composant | Spécification |
|-----------|---------------|
| CPU       | ESP32-S3 240MHz |
| RAM       | 320KB        |
| Flash     | 16MB         |
| Écran     | 320x240 IPS  |

## 💻 Installation

### Installation Rapide
```bash
# Cloner le projet
git clone https://github.com/ph3n4t3s/M5stackHuskyLens.git
cd M5stackHuskyLens

# Installer dépendances
pio pkg install

# Compiler et téléverser
pio run -t upload
```

### Connexions Matérielles
| M5Stack Core S3 | HuskyLens |
|----------------|-----------|
| Pin 2 (SDA)    | SDA      |
| Pin 1 (SCL)    | SCL      |
| 5V             | 5V       |
| GND            | GND      |

## ⚙️ Configuration

### Configuration Initiale
1. Premier démarrage : connexion au point d'accès "HuskyLens_XXXXXX"
2. Accès à http://192.168.4.1 pour la configuration
3. Configuration via interface web ou boutons physiques

### Contrôles Physiques
| Bouton | Action |
|--------|--------|
| A      | Navigation modes |
| B      | Modes affichage |
| C      | Capture/Apprentissage |
| A + C  | Menu configuration |

## 📖 Documentation
- [Guide d'Installation](docs/INSTALL.md)
- [Documentation API](docs/API.md)
- [Guide Développeur](docs/DEVELOPER.md)
- [Guide Utilisateur](docs/USER.md)
- [FAQ](docs/FAQ.md)

## 🎯 Performance
- RAM : 16.4% utilisé (53576/327680 bytes)
- Flash : 41.2% utilisé (1295577/3145728 bytes)
- FPS : 20+ en mode normal
- Temps réponse : <100ms
- Connexion I2C : 100kHz standard

## 🤝 Contribution
Les contributions sont les bienvenues ! Voici comment vous pouvez aider :

1. Fork le projet
2. Créer une branche (`git checkout -b feature/AmazingFeature`)
3. Commit les changements (`git commit -m 'Add AmazingFeature'`)
4. Push vers la branche (`git push origin feature/AmazingFeature`)
5. Ouvrir une Pull Request

## 📄 Licence
Ce projet est sous licence MIT - voir le fichier [LICENSE](LICENSE) pour plus de détails.

## 📧 Contact
Créé par [Ph3n4t3s](https://github.com/ph3n4t3s)

[⬆ Retour en haut](#m5stack-huskylens---système-de-vision-par-ordinateur-avancé)