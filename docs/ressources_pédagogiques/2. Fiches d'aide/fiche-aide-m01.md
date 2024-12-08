# Fiche d'aide : Mission 1 - Premier contact avec le matériel

## Contexte
Cette première mission consiste à installer et vérifier le bon fonctionnement du matériel qui servira à capturer les images de papillons pour le dataset du musée.

## Approche par la pensée computationnelle

### 1. Décomposition
Le problème d'installation peut être décomposé en plusieurs sous-tâches :
- Identification des composants
  * M5Stack Core S3 (unité principale)
  * Module HuskyLens (caméra intelligente)
  * Câble Grove (connexion)
  * Carte microSD (stockage)

- Séquence d'installation
  * Branchements physiques
  * Mise sous tension
  * Tests fonctionnels
  * Vérification stockage

### 2. Reconnaissance de motifs
Éléments répétitifs à identifier :
- Architecture des connecteurs
  * Format des ports Grove (détrompeur)
  * Orientation des connecteurs
  * Alignement des composants

- Signaux de fonctionnement
  * LEDs de status
  * Messages à l'écran
  * Indicateurs de connexion

### 3. Abstraction
Éléments essentiels à retenir :
- Principes de connexion
  * Le port Grove A est dédié au HuskyLens
  * Le câble Grove transporte données et alimentation
  * La carte SD doit être formatée en FAT32

- Points de contrôle
  * Solidité des connexions
  * Qualité de l'image
  * Réponse des boutons

### 4. Algorithme
Séquence logique d'installation :
1. Préparation
   - Déballer soigneusement chaque composant
   - Identifier les ports et connecteurs
   - Préparer la carte SD

2. Installation
   - Insérer la carte SD
   - Connecter le câble Grove au M5Stack
   - Connecter le HuskyLens
   - Vérifier la solidité des connexions

3. Premier démarrage
   - Allumer le M5Stack
   - Attendre l'initialisation
   - Vérifier l'écran
   - Contrôler les LEDs

4. Tests
   - Vérifier la communication I2C
   - Tester la caméra
   - Contrôler le stockage
   - Valider les fonctions de base

## Points clés de réussite

### Vérifications physiques
- [ ] Connecteurs propres et non endommagés
- [ ] Câble Grove correctement orienté
- [ ] Carte SD insérée complètement
- [ ] Absence de jeu dans les connexions

### Vérifications fonctionnelles
- [ ] Écran M5Stack actif et lisible
- [ ] LED HuskyLens bleue allumée
- [ ] Communication I2C établie
- [ ] Image caméra visible

### Solutions problèmes courants

#### Pas d'allumage
1. Vérifier la charge batterie
2. Maintenir le bouton power 5 secondes
3. Tester avec l'alimentation USB

#### HuskyLens non détecté
1. Vérifier l'orientation du câble Grove
2. Réinsérer le câble
3. Redémarrer le système

#### Carte SD non reconnue
1. Reformater en FAT32
2. Vérifier l'insertion complète
3. Essayer une autre carte

## Ressources

### Documentation technique
- Guide M5Stack Core S3
- Manuel HuskyLens
- Spécifications I2C

### Support pratique
- FAQ technique
- Forum communauté
- Service assistance

### Matériel supplémentaire
- Support de montage
- Chiffon nettoyage optique
- Câbles de rechange
