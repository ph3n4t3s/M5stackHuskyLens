# Ressources Mission 6 : Protocole de capture

## 1. Protocole de capture standardisé

```python
class CaptureProtocol:
    def __init__(self):
        self.standard_settings = {
            'lighting': {
                'natural': {
                    'min_lux': 500,
                    'max_lux': 1000,
                    'white_balance': 'auto',
                    'diffusion': 'required'
                },
                'studio': {
                    'main_light': '45° right',
                    'fill_light': '45° left',
                    'intensity': 800,
                    'color_temp': 5500
                }
            },
            'camera': {
                'resolution': (1920, 1080),
                'quality': 'high',
                'focus_mode': 'auto',
                'exposure_mode': 'matrix'
            },
            'distances': {
                'standard': 30,  # cm
                'macro': 15,     # cm
                'detail': 10     # cm
            },
            'angles': {
                'primary': [0, 45, 90],
                'secondary': [30, 60],
                'detail': [15, 75]
            }
        }

    def generate_capture_sequence(self, specimen_type):
        """
        Génère séquence de capture pour un spécimen
        """
        sequence = {
            'setup': self.get_setup_instructions(specimen_type),
            'captures': self.get_capture_sequence(specimen_type),
            'validation': self.get_validation_steps(specimen_type)
        }
        return sequence

    def get_setup_instructions(self, specimen_type):
        """
        Instructions setup spécifiques au type
        """
        return {
            'environment': self.get_environment_setup(),
            'equipment': self.get_equipment_setup(),
            'specimen': self.get_specimen_preparation(specimen_type)
        }

    def get_capture_sequence(self, specimen_type):
        """
        Séquence captures pour type spécimen
        """
        sequence = []
        for angle in self.standard_settings['angles']['primary']:
            sequence.append({
                'angle': angle,
                'distance': self.standard_settings['distances']['standard'],
                'settings': self.get_optimal_settings(angle, specimen_type)
            })
        return sequence
```

## 2. Guide technique de capture

```markdown
# Guide technique de capture

## 1. Configuration environnement

### Éclairage naturel
```
Setup naturel optimal:
     [Fenêtre]
        │
    45° ↓
[Diffuseur] → [Spécimen]
        ↑
    [Réflecteur]
```

#### Paramètres
- Intensité: 500-1000 lux
- Diffusion: panneau translucide
- Réflecteur: blanc neutre
- Heure optimale: 10h-14h

### Éclairage studio
```
Configuration studio:
[Key light] →  45°
               [Spécimen]
[Fill light] → 45°     ← [Back light]
```

#### Paramètres
- Key light: 800 lux, 45° droit
- Fill light: 400 lux, 45° gauche
- Back light: 200 lux (optionnel)
- Température couleur: 5500K

## 2. Positions capture

### Angles standards
```
Vue dessus:
       0°
       ↑
270° ← O → 90°
       ↓
      180°
```

#### Séquence capture
1. Vue frontale (0°)
   - Distance: 30cm
   - Focus: centre spécimen
   - Cadrage: symétrique

2. Vue oblique (45°)
   - Distance: 35cm
   - Focus: aile principale
   - Cadrage: dynamique

3. Vue latérale (90°)
   - Distance: 30cm
   - Focus: profil complet
   - Cadrage: horizontal

## 3. Paramètres techniques

### Configuration caméra
| Paramètre | Valeur standard | Valeur macro |
|-----------|----------------|--------------|
| Mode      | Auto           | Manuel       |
| Focus     | Centre         | Point        |
| Exposition| Matrice        | Spot         |
| ISO       | 100-400        | 100-200      |

### Réglages spéciaux
1. Mode HDR
   - Bracketing: ±1 EV
   - Alignement: auto
   - Fusion: naturelle

2. Mode Macro
   - Distance min: 15cm
   - Focus: manuel
   - Stack si nécessaire
```

## 3. Checklist de capture

```python
class CaptureChecklist:
    def __init__(self):
        self.checklist_items = {
            'pre_capture': {
                'environment': [
                    'Zone dégagée',
                    'Éclairage contrôlé',
                    'Température stable',
                    'Absence vibrations'
                ],
                'equipment': [
                    'Batterie chargée',
                    'Espace stockage suffisant',
                    'Optique propre',
                    'Paramètres vérifiés'
                ],
                'specimen': [
                    'Identification correcte',
                    'État optimal',
                    'Position préparée',
                    'Étiquetage prêt'
                ]
            },
            'during_capture': {
                'technical': [
                    'Focus vérifié',
                    'Exposition correcte',
                    'Balance blancs ajustée',
                    'Cadrage respecté'
                ],
                'workflow': [
                    'Séquence suivie',
                    'Métadonnées notées',
                    'Vérification directe',
                    'Backup immédiat'
                ]
            },
            'post_capture': {
                'validation': [
                    'Qualité vérifiée',
                    'Nombre images correct',
                    'Métadonnées complètes',
                    'Backup confirmé'
                ],
                'organization': [
                    'Fichiers nommés',
                    'Dossiers organisés',
                    'Documentation mise à jour',
                    'Matériel rangé'
                ]
            }
        }

    def generate_checklist(self, capture_type):
        """
        Génère checklist adaptée au type de capture
        """
        return {
            phase: {
                category: self.filter_items(items, capture_type)
                for category, items in phase_items.items()
            }
            for phase, phase_items in self.checklist_items.items()
        }
```

## 4. Guides spécifiques par condition

```markdown
# Guides spécialisés capture

## 1. Capture naturelle

### Conditions optimales
- Temps: clair mais couvert
- Heure: 10h-14h
- Vent: nul à faible
- Température: 20-25°C

### Procédure
1. Vérification conditions
   - Mesure lumière
   - Stabilité météo
   - Zone protégée

2. Installation
   - Position nord-sud
   - Diffuseur si nécessaire
   - Réflecteur en place

3. Capture
   - Test exposition
   - Série standard
   - Vérification régulière

## 2. Capture studio

### Setup studio
1. Éclairage principal
   - Position: 45° droite
   - Hauteur: 45° vertical
   - Intensité: 800 lux
   - Diffusion: softbox

2. Éclairage secondaire
   - Position: 45° gauche
   - Hauteur: 30° vertical
   - Intensité: 400 lux
   - Diffusion: parapluie

3. Configuration
   - Fond: neutre
   - Distance: 1m min
   - Réflecteurs: optionnels
   - Filtres: aucun

## 3. Capture macro

### Configuration spéciale
1. Support
   - Trépied stable
   - Rotule précise
   - Rail macro si nécessaire

2. Éclairage
   - Ring light
   - Flash macro
   - Diffusion maximale

3. Technique
   - Focus stacking
   - Exposition multiple
   - HDR si nécessaire
```

## 5. Outils de contrôle capture

```python
class CaptureController:
    def __init__(self):
        self.control_parameters = {
            'exposure': {
                'check': self.check_exposure,
                'adjust': self.adjust_exposure,
                'validate': self.validate_exposure
            },
            'focus': {
                'check': self.check_focus,
                'adjust': self.adjust_focus,
                'validate': self.validate_focus
            },
            'composition': {
                'check': self.check_composition,
                'adjust': self.adjust_composition,
                'validate': self.validate_composition
            },
            'lighting': {
                'check': self.check_lighting,
                'adjust': self.adjust_lighting,
                'validate': self.validate_lighting
            }
        }

    def monitor_capture(self, capture_settings):
        """
        Monitore une session de capture
        """
        monitoring = {
            'real_time': self.real_time_monitoring(),
            'alerts': self.check_alert_conditions(),
            'adjustments': self.suggest_adjustments(),
            'validation': self.continuous_validation()
        }
        return monitoring

    def generate_capture_report(self, session_data):
        """
        Génère rapport de session capture
        """
        report = {
            'summary': self.create_session_summary(session_data),
            'technical': self.analyze_technical_data(session_data),
            'quality': self.assess_capture_quality(session_data),
            'recommendations': self.make_recommendations(session_data)
        }
        return report
```

Ces ressources pour la Mission 6 fournissent :
1. Un protocole de capture détaillé et programmable
2. Des guides techniques complets
3. Des checklists exhaustives
4. Des guides spécifiques par condition
5. Des outils de contrôle et monitoring

Voulez-vous que je continue avec l'analyse et la création des ressources manquantes pour la Mission 7 ?