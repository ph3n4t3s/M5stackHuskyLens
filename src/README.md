# Structure du code source

## Organisation

```
src/
├── core/           # Fonctionnalités fondamentales
├── drivers/        # Pilotes matériels
├── interfaces/     # Interfaces utilisateur
├── services/       # Services système
├── utils/          # Utilitaires
└── web/           # Interface web et API
```

## Description des composants

### Core (`core/`)
Contient les fonctionnalités essentielles :
- Gestion de la configuration
- Système de logging
- Gestion des événements
- Système de plugins

### Drivers (`drivers/`)
Pilotes pour le matériel :
- HuskyLens
- Écran M5Stack
- Capteurs
- Communication I2C/UART

### Interfaces (`interfaces/`)
Interfaces utilisateur :
- Interface LCD
- Menu système
- Gestionnaire d'affichage
- Entrées utilisateur

### Services (`services/`)
Services système :
- Reconnaissance d'objets
- Machine Learning
- Analytics
- Gestion WiFi

### Utils (`utils/`)
Utilitaires communs :
- Gestion mémoire
- Outils mathématiques
- Traitement d'images
- Helpers divers

### Web (`web/`)
Interface web et API :
- Serveur web
- API REST
- WebSocket
- Interface de configuration