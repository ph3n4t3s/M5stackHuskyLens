# M5Stack HuskyLens v1.0.2 - Amélioration du Système de Build

## 🎉 Points Clés

Cette version apporte des améliorations significatives au système de build et à la gestion des dépendances, rendant l'installation et la compilation plus fiables sur toutes les plateformes.

### ✨ Nouveautés

- Nouveau système de build basé sur Python
- Configuration JSON pour les dépendances
- Installation automatisée plus robuste
- Documentation mise à jour

### 🔧 Améliorations Techniques

#### Système de Build
- Script Python pour la gestion des patches
- Configuration JSON pour les dépendances
- Meilleure intégration avec PlatformIO
- Gestion automatique des bibliothèques

#### Installation
- Processus simplifié
- Meilleure détection des erreurs
- Installation automatique des dépendances
- Vérification de l'environnement

### 📦 Installation

1. **Cloner le Projet**
   ```bash
   git clone https://github.com/ph3n4t3s/M5stackHuskyLens.git
   cd M5stackHuskyLens
   ```

2. **Installation des Dépendances**
   ```bash
   pio pkg install
   ```

3. **Compilation**
   ```bash
   pio run
   ```

### 🔍 Changements Détaillés

#### Nouveaux Scripts
- `scripts/pre_build.py` : Script principal de build
- `scripts/config.json` : Configuration des dépendances
- Amélioration de la gestion des patches

#### Documentation
- Guide d'installation mis à jour
- Instructions de build clarifiées
- Documentation technique enrichie

### 🐛 Corrections
- Résolution de l'erreur "platform-ide.build not found"
- Amélioration de la stabilité du build
- Correction des problèmes de dépendances

### 📊 Performance
- Temps de compilation optimisé
- Meilleure gestion de la mémoire
- Installation plus rapide

### 📝 Notes de Migration

Pour mettre à jour depuis une version précédente :

1. **Mise à jour du Code**
   ```bash
   git pull origin main
   ```

2. **Nettoyage**
   ```bash
   pio run -t clean
   ```

3. **Réinstallation**
   ```bash
   pio pkg install
   pio run
   ```

### 🔍 Compatibilité

- ✅ macOS sur Apple Silicon (M1/M2)
- ✅ macOS sur Intel
- ✅ Linux
- ✅ Windows
- ✅ ESP32-S3

### 📚 Documentation

- [Guide d'Installation](docs/INSTALL.md)
- [Documentation API](docs/API.md)
- [Guide Développeur](docs/DEVELOPER.md)
- [Guide Utilisateur](docs/USER.md)

### 👥 Contributeurs

- [Ph3n4t3s](https://github.com/ph3n4t3s)