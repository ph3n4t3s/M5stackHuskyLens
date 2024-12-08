# Fiches Solutions - Missions 5 & 6

## Mission 5 : Organisation du travail

### Solution détaillée

#### 1. Système de nommage
```python
class NamingSystem:
    def __init__(self):
        self.species_codes = {
            'Morpho bleu': 'MBLU',
            'Apollon': 'APOL',
            'Machaon': 'MACH',
            'Paon du jour': 'PAPI',
            'Vulcain': 'VULN'
        }
        
        self.condition_codes = {
            'naturel': 'NAT',
            'studio': 'STU',
            'macro': 'MAC',
            'hdr': 'HDR'
        }
    
    def generate_filename(self, species, date, sequence, condition):
        """
        Génère un nom de fichier standardisé
        Format: SPCE_YYYYMMDD_SSS_CND
        """
        species_code = self.species_codes[species]
        date_str = date.strftime('%Y%m%d')
        seq_str = f"{sequence:03d}"
        cond_code = self.condition_codes[condition]
        
        return f"{species_code}_{date_str}_{seq_str}_{cond_code}"
```

#### 2. Structure des dossiers
```python
def create_project_structure():
    """
    Crée la structure standard du projet
    """
    structure = {
        'raw': {
            'MBLU': ['images', 'metadata'],
            'APOL': ['images', 'metadata'],
            'MACH': ['images', 'metadata'],
            'PAPI': ['images', 'metadata'],
            'VULN': ['images', 'metadata']
        },
        'processed': {
            'MBLU': ['images', 'metadata'],
            'APOL': ['images', 'metadata'],
            'MACH': ['images', 'metadata'],
            'PAPI': ['images', 'metadata'],
            'VULN': ['images', 'metadata']
        },
        'metadata': {},
        'documentation': {
            'protocols': {},
            'reports': {},
            'tools': {}
        }
    }
    
    return create_directories(structure)
```

#### 3. Gestion des métadonnées
```python
class MetadataManager:
    def __init__(self):
        self.schema = {
            'required': [
                'image_id',
                'capture_date',
                'species',
                'technical_info'
            ],
            'optional': [
                'weather',
                'notes',
                'operator'
            ]
        }
    
    def create_metadata(self, image_info):
        """
        Crée un fichier metadata standardisé
        """
        metadata = {
            'image_id': image_info['filename'],
            'capture_date': datetime.now().isoformat(),
            'species': image_info['species'],
            'technical_info': {
                'device': 'HuskyLens',
                'resolution': '1920x1080',
                'format': 'jpg',
                'settings': get_current_settings()
            }
        }
        
        return validate_and_save_metadata(metadata)
```

### Points clés de réussite
1. Cohérence du nommage
2. Structure logique
3. Métadonnées complètes
4. Documentation à jour

## Mission 6 : Protocole de capture

### Solution détaillée

#### 1. Configuration capture
```python
class CaptureConfig:
    def __init__(self):
        self.settings = {
            'resolution': (1920, 1080),
            'quality': 90,
            'format': 'jpg',
            'auto_focus': True
        }
        
        self.lighting = {
            'natural': {
                'min_lux': 500,
                'max_lux': 1000,
                'white_balance': 'auto'
            },
            'studio': {
                'min_lux': 800,
                'max_lux': 1200,
                'white_balance': 'fixed'
            }
        }
    
    def configure_capture(self, mode):
        """
        Configure les paramètres de capture
        """
        setup_camera(self.settings)
        setup_lighting(self.lighting[mode])
        return verify_configuration()
```

#### 2. Protocole de prise de vue
```markdown
# Protocole capture standard

## Préparation
1. Configuration matériel
   □ Vérifier batterie
   □ Nettoyer optique
   □ Préparer support

2. Réglages initiaux
   □ Mode haute résolution
   □ Balance des blancs
   □ Focus automatique

3. Conditions environnement
   □ Éclairage stable
   □ Fond neutre
   □ Zone dégagée

## Séquence capture
1. Vue standard (0°)
   □ Distance: 30cm
   □ Angle: perpendiculaire
   □ Centrage: sujet centré

2. Vue oblique (45°)
   □ Distance: 35cm
   □ Angle: 45° vertical
   □ Cadrage: ailes visibles

3. Vue détail
   □ Distance: 15cm
   □ Mode: macro
   □ Focus: caractéristiques
```

#### 3. Contrôle qualité capture
```python
class CaptureQualityControl:
    def __init__(self):
        self.quality_checks = {
            'focus': check_focus_quality,
            'exposure': check_exposure,
            'framing': check_framing,
            'color': check_color_balance
        }
    
    def validate_capture(self, image):
        """
        Valide une capture en temps réel
        """
        results = {}
        for check_name, check_func in self.quality_checks.items():
            results[check_name] = check_func(image)
            
        return all(results.values())
    
    def generate_feedback(self, results):
        """
        Génère feedback immédiat
        """
        feedback = []
        for check, result in results.items():
            if not result:
                feedback.append(get_correction_advice(check))
        return feedback
```

### Outils pratiques

#### 1. Guide angles et distances
```python
class CaptureGuide:
    def __init__(self):
        self.angles = {
            'standard': 0,
            'oblique': 45,
            'lateral': 90
        }
        
        self.distances = {
            'overview': 50,
            'standard': 30,
            'macro': 15
        }
    
    def get_capture_guide(self, mode):
        """
        Retourne guide pour mode spécifique
        """
        return {
            'angle': self.angles[mode],
            'distance': self.distances[mode],
            'framing': get_framing_guide(mode)
        }
```

#### 2. Checklist capture
```markdown
# Checklist capture

## Avant capture
□ Vérification matériel
  ├── Batterie chargée
  ├── Carte SD disponible
  └── Optique propre

□ Configuration
  ├── Mode approprié
  ├── Paramètres corrects
  └── Test rapide

□ Environnement
  ├── Éclairage vérifié
  ├── Fond préparé
  └── Zone dégagée

## Pendant capture
□ Stabilité
□ Cadrage
□ Focus
□ Exposition

## Après capture
□ Vérification image
□ Sauvegarde
□ Documentation
```

### Solutions problèmes courants

#### 1. Guide dépannage
```python
class TroubleshootingGuide:
    def __init__(self):
        self.common_issues = {
            'blur': {
                'causes': ['mouvement', 'focus incorrect'],
                'solutions': ['utiliser support', 'recalibrer focus']
            },
            'exposure': {
                'causes': ['éclairage inadéquat'],
                'solutions': ['ajuster lumière', 'modifier paramètres']
            },
            'framing': {
                'causes': ['position incorrecte'],
                'solutions': ['utiliser guides', 'repositionner']
            }
        }
    
    def get_solution(self, issue):
        """
        Retourne solutions pour problème spécifique
        """
        if issue in self.common_issues:
            return self.common_issues[issue]
        return None
```

### Documentation capture

#### 1. Journal de capture
```markdown
# Journal capture

## Session [DATE]
Opérateur: [ID]
Conditions: [DESCRIPTION]

### Captures réalisées
1. [ID_CAPTURE]
   - Espèce: [NOM]
   - Mode: [TYPE]
   - Qualité: [SCORE]
   - Notes: [OBSERVATIONS]

### Problèmes rencontrés
- [DESCRIPTION]
  * Solution: [ACTION]
  * Résultat: [OUTCOME]

### Améliorations suggérées
1. [SUGGESTION]
   - Raison: [POURQUOI]
   - Impact: [EFFET]
```

#### 2. Rapport qualité
```python
def generate_quality_report(session_data):
    """
    Génère rapport qualité session
    """
    report = {
        'summary': {
            'total_captures': len(session_data),
            'successful': count_successful(session_data),
            'quality_score': calculate_average_quality(session_data)
        },
        'details': analyze_capture_details(session_data),
        'recommendations': generate_recommendations(session_data)
    }
    
    return format_report(report)
```

Cette version double des solutions pour les Missions 5 et 6 fournit :
1. Des outils pratiques et directement utilisables
2. Des guides détaillés et systématiques
3. Des solutions aux problèmes courants
4. Une documentation complète

Voulez-vous que je continue avec les solutions des Missions 7 et 8 ?