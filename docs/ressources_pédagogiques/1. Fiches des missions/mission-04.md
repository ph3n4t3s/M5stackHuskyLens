# Mission 4 : Qualité des données

## Briefing
Pour que l'IA puisse reconnaître efficacement les papillons, la qualité des images est cruciale. Vous devez apprendre à évaluer et assurer la qualité des données collectées.

## Objectifs d'apprentissage
- Définir les critères de qualité
- Évaluer les images
- Identifier les défauts
- Établir un processus de contrôle

## Critères de qualité

### 1. Qualité technique
#### Résolution
- Minimum : 1280x720
- Optimale : 1920x1080
- Maximum : 2560x1440

#### Netteté
- Focus automatique
- Absence de flou
- Détails visibles

#### Exposition
- Équilibrée
- Sans surexposition
- Sans zones sombres

### 2. Qualité du contenu
#### Cadrage
- Sujet centré
- Marges appropriées
- Angle correct

#### Contexte
- Fond neutre
- Absence d'éléments parasites
- Éclairage uniforme

#### Visibilité
- Caractéristiques distinctives visibles
- Couleurs fidèles
- Échelle reconnaissable

## Guide d'évaluation

### Processus d'évaluation
1. Contrôle technique initial
2. Évaluation du contenu
3. Validation des métadonnées
4. Décision finale

### Grille de notation

```
Qualité technique (50 points)
├── Résolution (/10)
├── Netteté (/20)
└── Exposition (/20)

Qualité contenu (50 points)
├── Cadrage (/20)
├── Contexte (/15)
└── Visibilité (/15)
```

## Exercices pratiques

### Exercice 1 : Évaluation technique
1. Analysez 10 images test
2. Utilisez la grille technique
3. Notez les problèmes
4. Proposez des améliorations

### Exercice 2 : Évaluation contenu
1. Examinez le cadrage
2. Vérifiez le contexte
3. Évaluez la visibilité
4. Documentez les observations

### Exercice 3 : Amélioration
1. Identifiez les défauts communs
2. Proposez des solutions
3. Testez les corrections
4. Validez les améliorations

## Outils d'évaluation

### Checklist qualité
- [ ] Résolution conforme
- [ ] Netteté optimale
- [ ] Exposition correcte
- [ ] Cadrage approprié
- [ ] Contexte propre
- [ ] Caractéristiques visibles

### Scripts d'analyse

```python
def check_image_quality(image_path):
    """
    Analyse la qualité technique d'une image
    Retourne un score sur 100
    """
    img = load_image(image_path)
    score = 0
    score += check_resolution(img)
    score += check_sharpness(img)
    score += check_exposure(img)
    return score
```

## Problèmes courants et solutions

### Problèmes techniques
1. Résolution insuffisante
   - Vérifier les paramètres
   - Ajuster la distance
   - Utiliser le mode HD

2. Flou
   - Stabiliser l'appareil
   - Utiliser l'autofocus
   - Augmenter l'éclairage

3. Mauvaise exposition
   - Ajuster l'exposition
   - Modifier l'éclairage
   - Utiliser le HDR

### Problèmes de contenu
1. Mauvais cadrage
   - Utiliser la grille
   - Centrer le sujet
   - Respecter les marges

2. Contexte inadéquat
   - Nettoyer l'arrière-plan
   - Uniformiser l'éclairage
   - Éliminer les ombres

## Documentation

### Rapport de qualité

```markdown
# Rapport d'évaluation
## Image : [ID]
- Score technique : /50
- Score contenu : /50
- Problèmes identifiés :
  * [Liste]
- Améliorations suggérées :
  * [Liste]
```

### Journal d'optimisation
- Date
- Images évaluées
- Problèmes résolus
- Améliorations apportées

## Ressources

### Documentation technique
- Guide d'évaluation
- Standards de qualité
- Méthodes d'optimisation

### Outils recommandés
- Analyseurs d'image
- Éditeurs basiques
- Scripts d'automatisation

## Prochaines étapes
Une fois la qualité maîtrisée, vous passerez à la Mission 5 : Organisation du travail, où vous apprendrez à structurer votre processus de collecte de données.
