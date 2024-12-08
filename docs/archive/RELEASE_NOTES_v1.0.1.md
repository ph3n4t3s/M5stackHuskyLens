# M5Stack HuskyLens v1.0.1 - Support macOS M1/M2

## 🎉 Nouveautés

Cette version apporte le support complet pour macOS sur Apple Silicon (M1/M2) et corrige plusieurs problèmes de compilation.

### ✨ Points Clés

- Support complet de macOS sur Apple Silicon
- Correction des problèmes de compilation
- Scripts d'installation automatisés
- Documentation mise à jour

### 🔧 Corrections Techniques

#### Compatibilité C++
- Résolution des conflits de types dans les fonctions min/max
- Amélioration de la compatibilité du compilateur
- Correction des problèmes de typage dans HuskyLens

#### Support macOS
- Scripts d'installation spécifiques
- Gestion automatique des dépendances
- Patches de compatibilité intégrés

### 📦 Installation sur macOS

1. **Installation Automatique**
   ```bash
   git clone https://github.com/ph3n4t3s/M5stackHuskyLens.git
   cd M5stackHuskyLens
   ./scripts/macos_setup.sh
   ```

2. **Compilation et Upload**
   ```bash
   pio run -t upload
   ```

### 🔍 Changements Détaillés

#### Nouveaux Scripts
- `scripts/macos_setup.sh` : Configuration automatique pour macOS
- `scripts/apply_patches.sh` : Application des patches de compatibilité
- Nouveaux patches pour la bibliothèque HuskyLens

#### Documentation
- Guide d'installation macOS
- Instructions de dépannage mises à jour
- Documentation de l'API mise à jour

### 🐛 Corrections de Bugs
- Résolution des conflits de types dans min/max
- Correction des erreurs de compilation sur macOS
- Amélioration de la stabilité générale

### 📊 Performance
- RAM : 16.4% utilisé (53576/327680 bytes)
- Flash : 41.2% utilisé (1295577/3145728 bytes)
- Compatible avec les processeurs Apple Silicon

### 📝 Notes de Migration

Pour migrer depuis la version 1.0.0 :

1. Mettre à jour le dépôt :
   ```bash
   git pull origin main
   ```

2. Exécuter le script de configuration :
   ```bash
   ./scripts/macos_setup.sh
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