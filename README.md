# M5Stack HuskyLens

## À propos

M5Stack HuskyLens est une solution complète de vision par ordinateur combinant la puissance du M5Stack CoreS3 et les capacités de reconnaissance du module HuskyLens. Cette solution permet la reconnaissance d'objets, de visages, et l'analyse en temps réel dans un package compact et facile à utiliser.

## Caractéristiques principales

- 🎯 Reconnaissance d'objets et de visages en temps réel
- 📱 Interface utilisateur intuitive sur écran LCD
- 🌐 Configuration et monitoring via interface web
- 📊 Analytics et métriques intégrés
- 🔄 Mise à jour Over-The-Air (OTA)
- 💾 Stockage local des données

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

### Mode autonome
1. Allumez l'appareil
2. Utilisez les boutons pour la navigation
3. Sélectionnez le mode de reconnaissance

### Mode connecté
1. Connectez-vous au réseau WiFi
2. Accédez à l'interface web
3. Configurez et monitorer à distance

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