# Ressources Mission 3 : Exploration des datasets

## 1. Dataset d'exemple commenté
```python
class ExampleDataset:
    def __init__(self):
        self.structure = {
            'root': 'papillons_dataset/',
            'categories': {
                'morpho_bleu': {
                    'description': 'Morpho bleu (Morpho peleides)',
                    'exemplaires': 10,
                    'conditions': ['naturel', 'studio']
                },
                'apollon': {
                    'description': 'Apollon (Parnassius apollo)',
                    'exemplaires': 10,
                    'conditions': ['naturel', 'studio']
                }
            },
            'metadata': {
                'version': '1.0',
                'date_creation': '2024-02-07',
                'auteur': 'MNHN',
                'licence': 'CC BY-SA 4.0'
            }
        }

    def generate_example_structure(self):
        """
        Génère la structure d'exemple
        """
        return {
            'dataset': {
                'images': self._generate_image_structure(),
                'metadata': self._generate_metadata_structure(),
                'documentation': self._generate_doc_structure()
            }
        }

    def _generate_image_structure(self):
        """
        Structure des images avec exemples
        """
        return {
            'raw': {
                'MBLU_20240207_001_NAT.jpg': {
                    'resolution': '1920x1080',
                    'format': 'JPEG',
                    'size': '2.4MB',
                    'metadata': 'MBLU_20240207_001_NAT.json'
                }
            },
            'processed': {
                'MBLU_20240207_001_NAT_PROC.jpg': {
                    'resolution': '1920x1080',
                    'format': 'JPEG',
                    'size': '1.8MB',
                    'processing': ['color_correction', 'noise_reduction']
                }
            }
        }
```

## 2. Guide d'analyse structurelle

```markdown
# Guide d'analyse de structure dataset

## 1. Organisation hiérarchique

### Niveau 1 : Structure racine
```
dataset/
├── raw/                 # Images brutes
├── processed/           # Images traitées
├── metadata/           # Métadonnées
└── documentation/      # Documentation
```

### Niveau 2 : Catégories
```
raw/
├── morpho_bleu/        # Espèce 1
│   ├── natural/        # Conditions naturelles
│   └── studio/         # Conditions studio
└── apollon/            # Espèce 2
    ├── natural/
    └── studio/
```

### Niveau 3 : Images et métadonnées
```
morpho_bleu/natural/
├── MBLU_20240207_001_NAT.jpg    # Image
├── MBLU_20240207_001_NAT.json   # Métadonnées
├── MBLU_20240207_002_NAT.jpg
└── MBLU_20240207_002_NAT.json
```

## 2. Conventions de nommage

### Format standard
- Code espèce (4 caractères)
- Date (YYYYMMDD)
- Séquence (3 chiffres)
- Condition (3 caractères)

### Exemples
```
MBLU_20240207_001_NAT.jpg
│    │        │   │
│    │        │   └── Condition (NAT/STU)
│    │        └────── Numéro séquence
│    └─────────────── Date capture
└──────────────────── Code espèce
```

## 3. Organisation métadonnées

### Format JSON standard
```json
{
    "image_id": "MBLU_20240207_001_NAT",
    "capture": {
        "date": "2024-02-07",
        "time": "14:30:00",
        "operator": "OP001",
        "location": "Studio 1"
    },
    "subject": {
        "species": "Morpho peleides",
        "code": "MBLU",
        "sex": "male",
        "size": "12cm"
    },
    "technical": {
        "device": "HuskyLens",
        "resolution": "1920x1080",
        "format": "jpg",
        "settings": {
            "exposure": "0.0",
            "focus": "auto",
            "white_balance": "auto"
        }
    }
}
```

## 4. Structure documentation
```
documentation/
├── README.md           # Vue d'ensemble
├── STRUCTURE.md        # Organisation
├── METADATA.md         # Format métadonnées
└── guides/
    ├── capture.md      # Guide capture
    ├── processing.md   # Guide traitement
    └── validation.md   # Guide validation
```

## 3. Outils d'analyse

```python
class DatasetAnalyzer:
    def __init__(self, dataset_path):
        self.dataset_path = dataset_path
        
    def analyze_structure(self):
        """
        Analyse complète de la structure
        """
        analysis = {
            'files': self.analyze_files(),
            'directories': self.analyze_directories(),
            'metadata': self.analyze_metadata(),
            'consistency': self.check_consistency()
        }
        return self.generate_report(analysis)
    
    def analyze_files(self):
        """
        Analyse des fichiers
        """
        files_analysis = {
            'total_count': 0,
            'by_type': {},
            'by_category': {},
            'size_distribution': {},
            'naming_compliance': {}
        }
        
        # Impl
        return files_analysis

    def analyze_metadata(self):
        """
        Analyse des métadonnées
        """
        metadata_analysis = {
            'completeness': self.check_metadata_completeness(),
            'consistency': self.check_metadata_consistency(),
            'validity': self.validate_metadata_format()
        }
        return metadata_analysis
```

## 4. Exemples pratiques commentés

```markdown
# Exemples pratiques d'organisation

## Exemple 1 : Dataset simple
### Structure
```
simple_dataset/
├── images/
│   ├── morpho_01.jpg
│   └── morpho_02.jpg
├── metadata.json
└── README.md
```
### Avantages
- Organisation simple
- Facile à comprendre
- Maintenance aisée

### Inconvénients
- Peu évolutif
- Organisation limitée
- Métadonnées centralisées

## Exemple 2 : Dataset hiérarchique
### Structure
```
hierarchical_dataset/
├── raw/
│   └── morpho/
│       ├── natural/
│       └── studio/
├── processed/
├── metadata/
└── docs/
```
### Avantages
- Organisation claire
- Évolutif
- Séparation données

### Inconvénients
- Plus complexe
- Navigation plus longue
- Gestion plus lourde

## Exemple 3 : Dataset distribué
### Structure
```
distributed_dataset/
├── site_1/
│   └── captures/
├── site_2/
│   └── captures/
└── consolidated/
```
### Avantages
- Multi-sources
- Traçabilité
- Flexibilité

### Inconvénients
- Synchronisation nécessaire
- Risque incohérence
- Gestion complexe
```

## 5. Checklist d'analyse

```markdown
# Checklist analyse dataset

## 1. Structure générale
□ Organisation logique des dossiers
□ Hiérarchie claire
□ Nommage cohérent
□ Documentation présente

## 2. Fichiers
□ Formats standards
□ Nommage conforme
□ Organisation cohérente
□ Tailles appropriées

## 3. Métadonnées
□ Format standard
□ Complétude
□ Cohérence
□ Validité

## 4. Documentation
□ README présent
□ Guide structure
□ Guide utilisation
□ Informations licence

## 5. Qualité
□ Standards respectés
□ Cohérence globale
□ Pas de données manquantes
□ Pas d'erreurs évidentes
```

Ces ressources pour la Mission 3 fournissent :
1. Un dataset d'exemple détaillé et commenté
2. Un guide complet d'analyse structurelle
3. Des outils d'analyse programmables
4. Des exemples pratiques variés
5. Une checklist d'analyse exhaustive

Voulez-vous que je continue avec l'analyse et la création des ressources manquantes pour la Mission 4 ?