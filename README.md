# M5Stack HuskyLens

[![Version](https://img.shields.io/badge/version-2.0.0-blue.svg)](https://github.com/ph3n4t3s/M5stackHuskyLens/releases)
[![Coût total](https://img.shields.io/badge/co%C3%BBt%20total-$574.63-green.svg)](docs/coast/README.md)
[![Période](https://img.shields.io/badge/p%C3%A9riode-Mar%202024--Dec%202024-lightgrey.svg)](docs/coast/README.md)

## À propos

M5Stack HuskyLens est une solution complète de vision par ordinateur combinant la puissance du M5Stack CoreS3 et les capacités de reconnaissance du module HuskyLens. Cette solution permet la reconnaissance d'objets, de visages, et l'analyse en temps réel dans un package compact et facile à utiliser.

## Caractéristiques principales

- 🎯 Reconnaissance d'objets et de visages en temps réel
- 📱 Interface utilisateur moderne et adaptative
- 🌐 Configuration simplifiée par QR Code
- 📊 Analytics et métriques intégrés
- 🌙 Mode nuit et contrôle de luminosité
- 💾 Sauvegarde automatique des paramètres
- 📋 Logs en temps réel au démarrage
- ⚙️ Configuration flexible accessible à tout moment
- 🔄 Mise à jour Over-The-Air (OTA)

## Prérequis

### Matériel
- M5Stack CoreS3
- Module HuskyLens
- Câble USB-C
- (Optionnel) Connectivité WiFi

### Logiciel
- PlatformIO Core (CLI ou IDE)
- Python 3.8+
- Git

## Installation rapide

1. Cloner le dépôt :
\`\`\`bash
git clone https://github.com/ph3n4t3s/M5stackHuskyLens.git
cd M5stackHuskyLens
\`\`\`

2. Installer les dépendances :
\`\`\`bash
pio pkg install
\`\`\`

3. Compiler et uploader :
\`\`\`bash
pio run -t upload
\`\`\`

## Configuration

1. Première utilisation :
   - Connectez-vous au point d'accès WiFi "M5HuskyLens"
   - Accédez à http://192.168.4.1
   - Suivez l'assistant de configuration

2. Configuration avancée :
   - Modifiez `config.json` selon vos besoins
   - Utilisez l'interface web pour les ajustements en temps réel

## Documentation

- [Spécifications techniques complètes](docs/TECHNICAL_SPECIFICATIONS.md)
- [Notes de version](CHANGELOG.md)

## Support des plateformes

| Plateforme | Status | Notes |
|------------|--------|-------|
| Linux      | ✅     | Supporté nativement |
| macOS Intel| ✅     | Supporté depuis v1.0.1 |
| macOS M1/M2| ✅     | Supporté depuis v1.1.0 |
| Windows    | ✅     | Drivers spécifiques requis |

## Utilisation

### Premier démarrage
1. Allumez l'appareil
2. Suivez la séquence de démarrage visuelle
3. Scannez le QR Code avec votre smartphone pour la configuration WiFi

### Utilisation normale
1. Utilisez le bouton A pour afficher/masquer le QR Code
2. Utilisez le bouton B pour accéder aux paramètres
3. Utilisez le bouton C pour sauvegarder les modifications

### Mode connecté
1. Connectez-vous au réseau WiFi via le QR Code
2. Accédez à l'interface web
3. Configurez et monitorez à distance

### Personnalisation
1. Accédez aux paramètres via le bouton B
2. Ajustez la luminosité et le mode nuit
3. Les paramètres sont sauvegardés automatiquement

## Contribution

1. Fork le projet
2. Créez votre branche (\`git checkout -b feature/AmazingFeature\`)
3. Committez vos changements (\`git commit -m 'Add AmazingFeature'\`)
4. Push vers la branche (\`git push origin feature/AmazingFeature\`)
5. Ouvrez une Pull Request

## Licence

Ce projet est sous licence MIT - voir le fichier [LICENSE](LICENSE) pour plus de détails.

## Contact

Project Link: [https://github.com/ph3n4t3s/M5stackHuskyLens](https://github.com/ph3n4t3s/M5stackHuskyLens)

## Analyse des coûts

Ce projet a été développé avec l'aide de Claude AI, ce qui nous permet d'avoir une transparence totale sur les coûts de développement.

### Résumé des coûts
- **Coût total** : $574.63
- **Période** : Mars 2024 - Décembre 2024
- **Pic d'utilisation** : Avril 2024 ($296.66)
- **Coût moyen mensuel** : $63.85

### Répartition
- Développement initial : $530.78 (92.4%)
- Optimisations : $17.86 (3.1%)
- Maintenance : $26.00 (4.5%)

Pour plus de détails, voir [l'analyse complète des coûts](docs/coast/README.md).