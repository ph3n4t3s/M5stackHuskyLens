# Mission 1 : Premier contact avec le matériel

## Briefing
En tant que nouvel expert en IA au Musée National d'Histoire Naturelle, votre première mission consiste à installer et vérifier le bon fonctionnement de votre équipement de capture d'images.

## Objectifs d'apprentissage
- Installer correctement le matériel
- Comprendre les connexions entre les composants
- Effectuer les premiers tests de fonctionnement
- Valider l'installation complète

## Matériel nécessaire
- 1× M5Stack Core S3
- 1× Module HuskyLens
- 1× Câble Grove 4 pins
- 1× Carte microSD (16GB minimum)
- 1× Câble USB-C
- 1× Support de montage (optionnel)

## Guide d'installation pas à pas

### 1. Préparation du matériel
- Déballez soigneusement tous les composants
- Vérifiez qu'aucun élément n'est endommagé
- Identifiez chaque connecteur et port

### 2. Connexion des composants
1. Localisez le port Grove A sur le M5Stack Core S3
2. Identifiez le port Grove sur le HuskyLens
3. Connectez le câble Grove 4 pins :

   ```
   M5Stack (Port A) -> Câble Grove -> HuskyLens
   Pin 1 (SCL) -----> Fil jaune -----> SCL
   Pin 2 (SDA) -----> Fil blanc -----> SDA
   5V ---------------> Fil rouge -----> 5V
   GND --------------> Fil noir -----> GND
   ```

### 3. Installation de la carte microSD
1. Formatez la carte en FAT32
2. Insérez-la dans le slot du M5Stack Core S3
3. Vérifiez qu'elle est bien en place

### 4. Premier démarrage
1. Appuyez sur le bouton POWER du M5Stack
2. Vérifiez que l'écran s'allume
3. Observez la LED du HuskyLens (doit être bleue)
4. Attendez l'initialisation complète

### 5. Vérification du système
- [ ] Écran M5Stack fonctionnel
- [ ] HuskyLens LED active
- [ ] Carte microSD détectée
- [ ] Communication I2C établie

## Points de contrôle
1. Le M5Stack démarre correctement
2. L'écran affiche l'interface principale
3. Le HuskyLens est détecté
4. La carte microSD est accessible

## Problèmes courants et solutions

### Le HuskyLens n'est pas détecté
1. Vérifiez les connexions du câble Grove
2. Redémarrez le M5Stack
3. Testez avec un autre câble Grove

### L'écran reste noir
1. Vérifiez la charge de la batterie
2. Maintenez le bouton POWER 5 secondes
3. Connectez le câble USB-C

### La carte microSD n'est pas reconnue
1. Reformatez en FAT32
2. Testez une autre carte
3. Nettoyez les contacts

## Exercices pratiques

### Exercice 1 : Vérification des connexions
1. Identifiez chaque fil du câble Grove
2. Vérifiez la correspondance avec les ports
3. Documentez les connexions

### Exercice 2 : Test de démarrage
1. Éteignez complètement le système
2. Redémarrez pas à pas
3. Notez la séquence de démarrage

### Exercice 3 : Diagnostic basique
1. Vérifiez les LEDs de statut
2. Testez les boutons du M5Stack
3. Contrôlez l'affichage

## Auto-évaluation

### Validation technique
- [ ] Installation matérielle complète
- [ ] Test de démarrage réussi
- [ ] Composants tous fonctionnels
- [ ] Documentation complétée

### Questions de compréhension
1. Quel est le rôle de chaque fil du câble Grove ?
2. Comment vérifier que le HuskyLens est bien détecté ?
3. Pourquoi la carte microSD doit-elle être en FAT32 ?

## Ressources complémentaires

### Documentation technique
- [Manuel M5Stack Core S3](lien)
- [Guide HuskyLens](lien)
- [Spécifications I2C](lien)

### Vidéos tutorielles
1. Installation pas à pas
2. Test du système
3. Diagnostic des problèmes

### Liens utiles
- Forum communauté M5Stack
- Support technique HuskyLens
- FAQ du projet

## Prochaines étapes
Une fois l'installation validée, vous pourrez passer à la Mission 2 : Maîtrise des outils, où vous apprendrez à utiliser les fonctions de base du système.
