# Documentation M5Stack HuskyLens

![Version](https://img.shields.io/badge/version-2.0.2-blue.svg)
![Build](https://img.shields.io/badge/build-in%20progress-yellow.svg)
![Documentation](https://img.shields.io/badge/docs-updating-blue.svg)

## ⚠️ État du Projet

Le projet est en phase active de développement. Les changements récents incluent :

- Réorganisation complète de la structure du projet
- Mise à jour des dépendances et versions
- Amélioration de la documentation technique
- Correction des problèmes de compilation

Pour plus de détails, consultez :
- [Journal de Progression](dev/PROGRESS_LOG.md)
- [Problèmes Connus](KNOWN_ISSUES.md)
- [Spécifications V2](TECHNICAL_SPECIFICATIONS_V2.md)

## Guide de la Documentation

Cette documentation est organisée en plusieurs sections pour répondre aux besoins des différents utilisateurs :

### 📘 Documentation Utilisateur
- [Guide d'Installation](user/INSTALL.md)
- [Guide de Démarrage Rapide](user/QUICKSTART.md)
- [Manuel Utilisateur](user/USER_GUIDE.md)
- [FAQ](user/FAQ.md)
- [Guide de Dépannage](user/TROUBLESHOOTING.md)

### 🔧 Documentation Technique
- [Spécifications V2](TECHNICAL_SPECIFICATIONS_V2.md)
- [Architecture Système](dev/ARCHITECTURE.md)
- [Guide du Développeur](dev/DEVELOPER.md)
- [Guide de Contribution](dev/CONTRIBUTING.md)
- [Guide de Style](dev/STYLE_GUIDE.md)
- [Guide de Test](dev/TESTING.md)
- [Journal de Progression](dev/PROGRESS_LOG.md)
- [Problèmes Connus](KNOWN_ISSUES.md)

### 🌐 Documentation API
- [Vue d'Ensemble de l'API](api/API_OVERVIEW.md)
- [Référence API REST](api/REST_API.md)
- [API Matérielle](api/HARDWARE_API.md)
- [API Vision](api/VISION_API.md)
- [API ML](api/ML_API.md)

### 📚 Ressources Supplémentaires
- [Changelog](CHANGELOG.md)
- [Licence](LICENSE.md)
- [Code de Conduite](CODE_OF_CONDUCT.md)
- [Support](SUPPORT.md)

## Navigation

- [Retour au README principal](../README.md)
- [Liste des Releases](https://github.com/ph3n4t3s/M5stackHuskyLens/releases)
- [Wiki du Projet](https://github.com/ph3n4t3s/M5stackHuskyLens/wiki)

## Comment Contribuer à la Documentation

1. Fork le projet
2. Créer une branche pour votre documentation
3. Écrire ou mettre à jour la documentation
4. Soumettre une Pull Request

### Style de Documentation

- Utiliser Markdown pour tout le contenu
- Inclure des exemples de code quand c'est pertinent
- Fournir des captures d'écran pour les interfaces utilisateur
- Maintenir une structure cohérente
- Vérifier l'orthographe et la grammaire
- Mettre à jour la table des matières

### Structure des Fichiers

```
docs/
├── api/                       # Documentation API
├── dev/                       # Documentation développeur
│   ├── PROGRESS_LOG.md       # Journal de progression
│   ├── ARCHITECTURE.md       # Architecture système
│   └── ...
├── user/                      # Documentation utilisateur
├── images/                    # Images et captures d'écran
├── TECHNICAL_SPECIFICATIONS_V2.md
├── KNOWN_ISSUES.md
└── index.md                   # Ce fichier

src/                          # Code source
├── api/                      # API REST et serveur web
├── core/                     # Composants fondamentaux
├── display/                  # Gestion de l'affichage
├── system/                   # Gestion système
└── utils/                    # Utilitaires
```

## Mise à Jour de la Documentation

### Version 2.0.2 (7 décembre 2024)

#### Changements Majeurs
- Réorganisation complète de la documentation
- Ajout du journal de progression (PROGRESS_LOG.md)
- Documentation des problèmes connus (KNOWN_ISSUES.md)
- Nouvelles spécifications techniques (V2)

#### En Cours
- Migration de l'ancienne documentation
- Mise à jour des guides utilisateur
- Enrichissement des exemples de code
- Traduction des documents principaux

### Processus de Mise à Jour
- Les mises à jour majeures sont documentées dans le CHANGELOG
- Les modifications quotidiennes sont dans le journal de progression
- Les problèmes connus sont mis à jour en continu
- La documentation technique est revue à chaque release

### Prochaines Étapes
1. Compléter la migration vers la nouvelle structure
2. Ajouter plus d'exemples de code
3. Améliorer la documentation API
4. Mettre à jour les guides utilisateur