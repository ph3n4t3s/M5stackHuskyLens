# Guide de contribution

## Prérequis

### Environnement de développement
- PlatformIO
- Git
- Python 3.8+
- (Optionnel) VSCode avec extension PlatformIO

### Connaissances recommandées
- C++ pour ESP32
- Arduino Framework
- Protocoles I2C/UART
- Développement web (pour l'interface)

## Workflow de développement

1. **Préparation**
   ```bash
   # Fork et clone
   git clone https://github.com/votre-username/M5stackHuskyLens.git
   cd M5stackHuskyLens
   
   # Créer une branche
   git checkout -b feature/ma-fonctionnalite
   ```

2. **Développement**
   ```bash
   # Installer les dépendances
   pio pkg install
   
   # Compiler
   pio run
   
   # Tester
   pio test
   ```

3. **Tests**
   - Unitaires : `test/unit/`
   - Intégration : `test/integration/`
   - End-to-end : `test/e2e/`

4. **Documentation**
   - Mise à jour du README si nécessaire
   - Documentation du code (Doxygen)
   - Mise à jour du CHANGELOG

## Standards de code

### Style
```cpp
// Exemple de style
class MyClass {
public:
    MyClass();
    void doSomething();

private:
    int m_variable;
};
```

### Nommage
- Classes : `PascalCase`
- Méthodes : `camelCase`
- Variables : `camelCase`
- Constantes : `UPPER_CASE`

### Documentation
```cpp
/**
 * @brief Description brève
 * 
 * Description détaillée
 * 
 * @param param1 Description du paramètre
 * @return Description du retour
 */
```

## Process de Pull Request

1. **Préparation**
   - Code propre et commenté
   - Tests passants
   - Documentation à jour

2. **Soumission**
   - Titre clair
   - Description détaillée
   - Screenshots si UI/UX

3. **Review**
   - Répondre aux commentaires
   - Mettre à jour si nécessaire
   - Être patient et constructif

## Types de contributions

### Bug fixes
```
fix: description courte

Description détaillée du bug et de la solution
```

### Nouvelles fonctionnalités
```
feat: description courte

Description détaillée de la fonctionnalité
```

### Documentation
```
docs: description courte

Description des changements dans la documentation
```

## Support

- Issues GitHub
- Discussions
- Pull Requests

## Licence

En contribuant, vous acceptez que votre code soit sous licence MIT.