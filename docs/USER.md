# Guide Utilisateur

## Mise en Route

### Installation du Matériel
1. Connexions
   - HuskyLens → M5Stack Core S3
     * SDA (Pin 2)
     * SCL (Pin 1)
     * 5V
     * GND

2. Première Utilisation
   - Allumer le M5Stack
   - Attendre l'initialisation
   - Suivre l'assistant de configuration

### Configuration WiFi
1. Connexion initiale
   - SSID : HuskyLens_XXXXXX
   - Mot de passe : huskyconfig
   - URL : http://192.168.4.1

2. Configuration réseau
   - Saisir SSID et mot de passe
   - Valider la connexion
   - Noter l'adresse IP affichée

## Interface Utilisateur

### Contrôles Physiques

#### Navigation
- **Bouton A** (Gauche)
  * Appui simple : Change le mode
  * Double appui : Menu précédent
  * Maintien : Retour accueil

- **Bouton B** (Centre)
  * Appui simple : Change l'affichage
  * Double appui : Export données
  * Maintien : Mode configuration

- **Bouton C** (Droite)
  * Appui simple : Action/Capture
  * Double appui : Apprentissage
  * Maintien : Menu avancé

#### Combinaisons
- **A + B** : Screenshot
- **B + C** : Réinitialisation
- **A + C** : Menu configuration

### Interface Web

#### Page d'Accueil
```
┌─────────────────────────────┐
│ Status: Connected           │
│ Mode: Face Recognition      │
│ Objects: 2                  │
├─────────────────────────────┤
│    [Live View]              │
│                             │
├─────────────────────────────┤
│ Controls                    │
│ [Mode▼] [Action] [Settings] │
└─────────────────────────────┘
```

#### Configuration
```
┌─────────────────────────────┐
│ Settings                    │
├─────────────────────────────┤
│ ☐ Auto-Learn               │
│ ☐ Night Mode               │
│ ☐ Motion Stabilization     │
│                             │
│ Sensitivity: [▒▒▒░░] 60%    │
│ Zoom: [▒░░░░] 20%           │
└─────────────────────────────┘
```

## Modes de Fonctionnement

### 1. Reconnaissance Faciale
- Apprentissage
  * Positionner le visage
  * Appuyer sur C
  * Attendre la confirmation

- Utilisation
  * Mode automatique
  * Labels sur les visages reconnus
  * Statistiques en temps réel

### 2. Suivi d'Objets
- Sélection
  * Cadrer l'objet
  * Appuyer sur C
  * L'objet est suivi

- Multi-objets
  * Mode automatique
  * Jusqu'à 10 objets
  * Identification unique

### 3. Vision Nocturne
- Activation
  * Menu ou automatique
  * Optimisation auto
  * Réduction bruit

- Paramètres
  * Sensibilité IR
  * Contraste
  * Luminosité

### 4. Reconnaissance de Gestes
- Apprentissage
  * Mode geste
  * Effectuer le geste
  * Nommer et sauver

- Utilisation
  * Détection auto
  * Feedback visuel
  * Actions associées

## Automatisation

### Création de Règles
1. Menu Automatisation
2. Nouvelle Règle
3. Configurer :
   - Déclencheur
   - Conditions
   - Actions
4. Activer

### Exemples de Règles
```
Si (personne détectée)
  ET (nuit)
Alors
  - Activer vision nocturne
  - Envoyer notification
```

```
Si (geste "cercle")
  ET (mode = suivi)
Alors
  - Changer mode
  - Capture écran
```

## Analytics

### Visualisation
- Graphiques temps réel
- Historique détections
- Statistiques performances

### Export Données
1. Format CSV
   ```
   timestamp,type,confidence,x,y
   12:34:56,face,0.95,120,80
   12:34:57,object,0.87,150,90
   ```

2. Format JSON
   ```json
   {
     "detections": [
       {
         "time": "12:34:56",
         "type": "face",
         "confidence": 0.95,
         "position": {
           "x": 120,
           "y": 80
         }
       }
     ]
   }
   ```

## Maintenance

### Mise à Jour
1. Interface Web
   - Menu System
   - Check Updates
   - Install

2. Carte SD
   - Copier firmware.bin
   - Redémarrer
   - Installation auto

### Sauvegarde
1. Configuration
   - Export settings
   - Backup sur SD
   - Cloud backup

2. Données
   - Export CSV/JSON
   - Automatic backup
   - Cloud sync

### Dépannage

#### Problèmes Courants
1. Connexion perdue
   - Vérifier câbles
   - Redémarrer
   - Reset I2C

2. Mauvaise détection
   - Nettoyer lentille
   - Ajuster sensibilité
   - Recalibrer

3. Performances
   - Réduire résolution
   - Désactiver fonctions
   - Clear logs

#### Diagnostics
1. Logs système
   - Menu Debug
   - Export logs
   - Analyse

2. Tests intégrés
   - Self-test
   - Calibration
   - Benchmark

## Bonnes Pratiques

### Reconnaissance Faciale
- Bon éclairage
- Distance optimale
- Angles variés

### Suivi d'Objets
- Contraste suffisant
- Mouvement modéré
- Zone dégagée

### Vision Nocturne
- Source IR stable
- Zone adaptée
- Calibration régulière

### Gestes
- Mouvements nets
- Zone libre
- Répétition apprentissage