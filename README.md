# M5Stack HuskyLens - Système de Vision Avancé

## Vue d'ensemble
Système de vision par ordinateur avancé combinant le M5Stack Core S3 et le module HuskyLens, enrichi de fonctionnalités d'IA, de traitement d'image en temps réel et d'automatisation.

## Fonctionnalités principales

### 1. Vision et Reconnaissance
- Reconnaissance faciale multi-personnes
- Suivi d'objets en temps réel
- Détection de lignes et intersections
- Reconnaissance de couleurs et formes
- Détection de tags et QR codes
- Vision nocturne adaptative
- Stabilisation d'image en temps réel

### 2. Intelligence Artificielle
- Apprentissage automatique embarqué (TensorFlow Lite)
- Classification d'objets personnalisée
- Reconnaissance de gestes avancée
- Détection de motifs complexes
- Analyse comportementale

### 3. Traitement d'Image
- Amélioration automatique de la qualité
- Filtres configurables en temps réel
- Réduction du bruit intelligente
- Correction de la luminosité adaptative
- Détection de contours avancée
- Mode vision nocturne

### 4. Automatisation
- Système de règles personnalisables
- Actions conditionnelles
- Déclencheurs multiples
- Planification temporelle
- Réponses automatisées

### 5. Connectivité
- Interface Web en temps réel
- API REST pour le contrôle
- Streaming vidéo
- Sauvegarde dans le cloud
- Notifications push

### 6. Analytics
- Analyse statistique en temps réel
- Génération de rapports
- Historique des détections
- Métriques de performance
- Export de données

## Technologies Utilisées
- TensorFlow Lite pour l'inférence IA
- EloquentTinyML pour l'apprentissage embarqué
- FastLED pour le traitement d'image optimisé
- AsyncWebServer pour l'interface web
- ArduinoJson pour la gestion des données
- FreeRTOS pour le multitâche

## Nouveaux Systèmes

### 1. Système de Vision Nocturne
- Amélioration adaptative de la luminosité
- Réduction du bruit intelligente
- Mode infrarouge automatique
- Détection en faible luminosité

### 2. Système d'Automatisation
- Moteur de règles flexible
- Actions personnalisables
- Conditions complexes
- Historique des déclenchements

### 3. Système ML Embarqué
- Classification en temps réel
- Apprentissage incrémental
- Gestion des modèles
- Prédictions optimisées

### 4. Traitement d'Image Avancé
- Filtres matriciels personnalisables
- Stabilisation de mouvement
- Détection de contours adaptative
- Amélioration automatique

## Guide d'Installation et d'Utilisation
Voir le [Guide d'Installation Détaillé](README_INSTALL.md) pour les instructions complètes d'installation, de configuration et d'utilisation.

## Prérequis
- M5Stack Core S3
- Module HuskyLens
- PlatformIO IDE
- Carte micro SD (optionnelle)
- Câbles de connexion I2C

## Installation Rapide
```bash
# Cloner le projet
git clone https://github.com/ph3n4t3s/M5stackHuskyLens.git
cd M5stackHuskyLens

# Installer les dépendances
pio pkg install

# Compiler et téléverser
pio run -t upload
```

## Configuration Rapide
1. Première utilisation : connectez-vous au point d'accès WiFi "HuskyLens_XXXXXX"
2. Accédez à http://192.168.4.1 pour la configuration
3. Utilisez l'interface web ou les boutons physiques pour la configuration

## Documentation
La documentation complète inclut :
- [Guide d'Installation](README_INSTALL.md)
- [Référence API](docs/API.md)
- [Guide Développeur](docs/DEVELOPER.md)
- [Guide Utilisateur](docs/USER.md)

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

Ce cahier des charges sert de référence pour la poursuite du développement pour permettre d'assurer une implémentation cohérente et complète du système.


# Historique

## 1. Situation actuelle
- Le projet vise à créer une interface entre M5Stack Core S3 et HuskyLens
- Principales fonctionnalités implémentées :
  * Reconnaissance faciale
  * Suivi d'objets
  * Suivi de lignes
  * Reconnaissance de couleurs
  * Reconnaissance de tags
  * Mesure de distance
  * Interface graphique personnalisable

## 2. Problèmes résolus
- Erreurs de compilation liées aux types min/max
- Fonction processStandardData manquante
- Problèmes de compatibilité avec la bibliothèque HuskyLens

## 3. Points à développer
- Amélioration de l'interface utilisateur
- Ajout de nouvelles fonctionnalités de détection
- Optimisation des performances
- Meilleure gestion de la mémoire

## 4. Connexions matérielles
- M5Stack Core S3 connecté au HuskyLens via I2C
- SDA sur Pin 2
- SCL sur Pin 1

## 5. Commandes de base
- Bouton A : Change le mode HuskyLens
- Bouton B : Change le mode d'affichage
- Bouton C : Capture/Apprentissage
- A + C : Menu de configuration

## 6. Pistes d'amélioration
1. Améliorer la stabilité du code
2. Ajouter de nouvelles fonctionnalités
3. Optimiser l'interface utilisateur
4. Implémenter la sauvegarde des configurations
5. Ajouter des modes de détection personnalisés
