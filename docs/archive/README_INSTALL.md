# Guide d'Installation et d'Utilisation

## Installation

### Prérequis
- PlatformIO IDE
- Git
- M5Stack Core S3
- Module HuskyLens
- Carte micro SD (optionnelle, pour le logging)
- Câbles de connexion I2C

### Installation du Matériel
1. Connecter le HuskyLens au M5Stack Core S3 :
   - SDA → Pin 2
   - SCL → Pin 1
   - 5V → 5V
   - GND → GND

2. Insérer une carte micro SD (optionnel)

### Installation du Logiciel
1. Cloner le projet :
   ```bash
   git clone https://github.com/ph3n4t3s/M5stackHuskyLens.git
   cd M5stackHuskyLens
   ```

2. Installer les dépendances :
   ```bash
   pio pkg install
   ```

3. Compiler et téléverser :
   ```bash
   pio run -t upload
   ```

## Configuration

### Configuration WiFi
1. À la première utilisation, le système crée un point d'accès WiFi :
   - SSID : HuskyLens_XXXXXX
   - Mot de passe : huskyconfig

2. Se connecter et accéder à http://192.168.4.1
3. Configurer les paramètres WiFi dans l'interface web

### Configuration du Système
1. Interface Web (http://[IP_APPAREIL])
   - Paramètres de vision
   - Règles d'automatisation
   - Configuration ML
   - Gestion des filtres

2. Configuration Matérielle (boutons M5Stack)
   - Bouton A : Navigation modes
   - Bouton B : Modes d'affichage
   - Bouton C : Actions/Apprentissage
   - A + C : Menu configuration

## Utilisation

### Modes de Vision
1. Reconnaissance Faciale
   - Apprentissage : Maintenir C devant un visage
   - Oubli : Double-clic sur C

2. Suivi d'Objets
   - Sélection : Clic sur C
   - Multi-objets : Mode automatique

3. Vision Nocturne
   - Activation : Menu ou automatique
   - Ajustement : Automatique ou manuel

4. Gestes
   - Apprentissage : Menu gestes
   - Reconnaissance : Automatique

### Interface Web

#### Page Principale
- Visualisation en direct
- Contrôles rapides
- Statut système
- Alertes

#### Configuration
- Paramètres vision
- Règles automatisation
- Modèles ML
- Filtres d'image

#### Analytics
- Historique détections
- Graphiques performance
- Export données
- Journaux système

## Développement

### Structure du Projet
```
M5stackHuskyLens/
├── src/
│   ├── main.cpp
│   ├── Config.h
│   ├── HuskyLensPlus.*
│   ├── ImageProcessor.*
│   ├── MLSystem.*
│   ├── AutomationSystem.*
│   └── ...
├── data/
│   ├── index.html
│   ├── ml_models/
│   └── config/
└── platformio.ini
```

### Ajout de Fonctionnalités
1. Nouveaux Filtres
   ```cpp
   void addCustomFilter(const String& name, const float kernel[3][3]);
   ```

2. Règles Automatisation
   ```cpp
   Rule rule;
   rule.name = "CustomRule";
   rule.conditions.push_back(Condition(...));
   rule.actions.push_back(Action(...));
   automationSystem.addRule(rule);
   ```

3. Modèles ML
   ```cpp
   mlSystem.addModel("customModel", weights, inputSize, outputSize);
   ```

## Dépannage

### Problèmes Courants
1. Connexion HuskyLens
   - Vérifier câblage I2C
   - Redémarrer les appareils
   - Vérifier adresse I2C

2. Vision Nocturne
   - Calibration automatique
   - Ajuster sensibilité
   - Vérifier éclairage IR

3. Performances
   - Vérifier utilisation mémoire
   - Ajuster résolution
   - Optimiser filtres

### Maintenance
1. Mise à jour firmware :
   ```bash
   pio run -t upload
   ```

2. Sauvegarde configuration :
   - Via interface web
   - Export sur carte SD
   - Backup cloud

3. Nettoyage logs :
   - Interface web
   - Commande système
   - Rotation automatique

## Contribution
1. Fork le projet
2. Créer une branche
3. Commiter les modifications
4. Pousser vers la branche
5. Créer une Pull Request

## Support
- Issues GitHub
- Documentation Wiki
- Forum communauté