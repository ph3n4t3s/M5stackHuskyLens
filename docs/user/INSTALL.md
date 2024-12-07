# Guide d'Installation

Ce guide vous guidera à travers le processus d'installation complet du système M5Stack HuskyLens.

## Table des Matières

- [Prérequis](#prérequis)
- [Installation Matérielle](#installation-matérielle)
- [Installation Logicielle](#installation-logicielle)
- [Configuration](#configuration)
- [Vérification](#vérification)
- [Dépannage](#dépannage)

## Prérequis

### Matériel Requis
- M5Stack Core S3
- Module HuskyLens
- Câbles de connexion I2C
- Câble USB-C pour la programmation
- Carte micro SD (optionnelle, pour le logging)
- Support de montage (optionnel)

### Logiciel Requis
- VSCode avec extension PlatformIO
- Git
- Python 3.7+
- Pilotes USB (si nécessaire)

## Installation Matérielle

### Étape 1 : Préparation
1. Inspecter le matériel pour d'éventuels dommages
2. Vérifier la compatibilité des connecteurs
3. Préparer un espace de travail propre

### Étape 2 : Connexions
1. **Connexion HuskyLens ↔ M5Stack Core S3**
   ```
   M5Stack     HuskyLens
   Pin 2   →   SDA
   Pin 1   →   SCL
   5V      →   5V
   GND     →   GND
   ```

2. **Vérification des Connexions**
   - Vérifier la solidité des connexions
   - S'assurer qu'il n'y a pas de court-circuit
   - Vérifier l'alignement des broches

### Étape 3 : Montage (optionnel)
1. Fixer le support de montage
2. Installer le module HuskyLens
3. Sécuriser les câbles

## Installation Logicielle

### Étape 1 : Préparation de l'Environnement
```bash
# Installation des outils nécessaires
sudo apt-get update
sudo apt-get install git python3 python3-pip

# Installation de PlatformIO CLI
pip3 install -U platformio
```

### Étape 2 : Récupération du Code
```bash
# Cloner le dépôt
git clone https://github.com/ph3n4t3s/M5stackHuskyLens.git
cd M5stackHuskyLens

# Installer les dépendances
pio pkg install
```

### Étape 3 : Compilation et Upload
```bash
# Compilation
pio run

# Upload
pio run -t upload
```

## Configuration

### Configuration WiFi Initiale
1. Allumer le M5Stack
2. Se connecter au réseau "HuskyLens_XXXXXX"
   - SSID : HuskyLens_XXXXXX
   - Mot de passe : huskyconfig
3. Accéder à http://192.168.4.1
4. Suivre l'assistant de configuration

### Configuration Système
1. **Via Interface Web**
   - Paramètres vision
   - Automatisation
   - Connectivité
   - Sécurité

2. **Via M5Stack**
   - Utiliser les boutons A/B/C
   - Naviguer dans les menus
   - Configurer les paramètres basiques

## Vérification

### Tests Post-Installation
1. **Test de Base**
   - Démarrage correct
   - Affichage fonctionnel
   - Connexion I2C stable

2. **Test Vision**
   - Reconnaissance faciale
   - Suivi d'objets
   - Détection de lignes
   - Vision nocturne

3. **Test Connectivité**
   - WiFi
   - Interface Web
   - API REST

### Checklist de Vérification
- [ ] Démarrage système OK
- [ ] Connexion HuskyLens OK
- [ ] WiFi fonctionnel
- [ ] Interface web accessible
- [ ] Vision fonctionnelle
- [ ] Logs système normaux

## Dépannage

### Problèmes Courants

1. **Erreur de Connexion I2C**
   - Vérifier les connexions physiques
   - Tester avec un multimètre
   - Vérifier l'adresse I2C

2. **Problèmes WiFi**
   - Vérifier les identifiants
   - Redémarrer le système
   - Réinitialiser la configuration

3. **Erreurs de Vision**
   - Calibrer la caméra
   - Vérifier l'éclairage
   - Mettre à jour le firmware

### Logs et Diagnostics
```bash
# Accès aux logs
pio device monitor

# Diagnostic système
http://[IP_APPAREIL]/diagnostic
```

### Support
- [Forum Communauté](https://forum.example.com)
- [Issues GitHub](https://github.com/ph3n4t3s/M5stackHuskyLens/issues)
- [Documentation Wiki](https://github.com/ph3n4t3s/M5stackHuskyLens/wiki)