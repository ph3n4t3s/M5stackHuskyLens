# Changelog

## [1.1.1] - 2024-12-08
### Added
- Analyse détaillée des coûts du projet
- Scripts d'analyse des coûts Claude AI
- Documentation des coûts et métriques
- Historique complet des interactions

### Changed
- Réorganisation de la documentation des coûts
- Ajout des données de coûts historiques
- Optimisation de l'utilisation de l'API

## [1.1.0] - 2024-12-08
### Added
- Support complet pour macOS M2
- Amélioration majeure du système de build
- Documentation technique détaillée
- Interface de mise à jour intégrée
- Nouveau système de compilation automatisé

### Changed
- Refonte complète de la documentation
- Restructuration des dossiers du projet
- Amélioration de la gestion des erreurs
- Mise à jour du système de build
- Optimisation de la compilation sur M1/M2

### Fixed
- Correction des problèmes de compilation sur macOS M2
- Résolution des conflits de types dans HuskyLens
- Correction des problèmes de chemins
- Amélioration de la stabilité générale

## [1.1.0] - 2024-12-08
### Added
- Support complet pour macOS M2
- Amélioration majeure du système de build
- Interface de mise à jour intégrée

### Changed
- Refonte du script pre_build.py pour une meilleure compatibilité
- Amélioration de la gestion des erreurs
- Mise à jour de la documentation

### Fixed
- Correction des problèmes de compilation sur macOS M2
- Résolution des problèmes de chemins sur différents systèmes d'exploitation
- Correction des problèmes de synchronisation Git

## [1.0.2] - 2024-12-07

### Améliorations
- Nouveau système de build avec script Python
- Configuration JSON pour les dépendances
- Meilleure gestion des patches
- Installation automatisée des dépendances

### Corrections
- Résolution de l'erreur "platform-ide.build not found"
- Amélioration de la stabilité du build système
- Optimisation du processus de compilation

## [1.0.1] - 2024-12-07

### Améliorations
- Support pour macOS sur Apple Silicon (M1/M2)
- Corrections de compatibilité pour le compilateur C++
- Script automatique d'application des patches
- Meilleure gestion des dépendances

### Corrections
- Résolution des conflits de types dans les fonctions min/max
- Correction des problèmes de compilation sur macOS
- Amélioration de la stabilité de la bibliothèque HuskyLens

## [1.0.0] - 2024-12-07

### Ajouts
- Interface complète entre M5Stack Core S3 et HuskyLens
- Système de vision par ordinateur avec modes multiples
- Interface utilisateur sur écran LCD
- Interface web de configuration
- API REST pour contrôle à distance
- Documentation complète
- Tests unitaires et d'intégration

### Fonctionnalités
- Reconnaissance faciale multi-personnes
- Suivi d'objets en temps réel
- Détection de lignes et intersections
- Reconnaissance de couleurs et formes
- Détection de tags et QR codes
- Vision nocturne adaptative
- Stabilisation d'image

### Améliorations Techniques
- Optimisation de la mémoire
- Système multi-tâches optimisé
- Gestion efficace des ressources
- Architecture modulaire
- Support du WiFi et du Bluetooth
- Interface I2C stable

### Corrections
- Résolution des problèmes de type min/max dans HuskyLens.h
- Correction des dépendances ESPAsyncWebServer
- Amélioration de la stabilité du système
- Optimisation de la consommation mémoire

### Documentation
- Documentation utilisateur complète
- Guide du développeur détaillé
- Documentation API REST
- Guide d'installation
- Guide de contribution

### Performance
- RAM : 16.4% utilisé (53576/327680 bytes)
- Flash : 41.2% utilisé (1295577/3145728 bytes)
- FPS : 20+ en mode normal
- Temps réponse : <100ms

### Notes de Développement
- Tests unitaires implémentés
- CI/CD configuré
- Guidelines de code établies
- Workflow de développement documenté