# Bibliothèque Quorum MQTT - M5Stack HuskyLens V2.0

## 1. Vue d'ensemble

### 1.1 Objectif
Cette bibliothèque permet aux étudiants d'interagir avec le M5Stack et HuskyLens via MQTT en utilisant le langage Quorum.

### 1.2 Fonctionnalités principales
- Connection au broker MQTT
- Publication et souscription aux topics
- Contrôle du M5Stack
- Gestion des événements HuskyLens
- Traitement des réponses asynchrones

## 2. Structure de la bibliothèque

### 2.1 Classes principales

#### MQTTClient
```quorum
class MQTTClient
    // Propriétés privées
    private text host = ""
    private integer port = 0
    private text clientId = ""
    private boolean connected = false
    
    // Constructeur
    public action Create(text host, integer port)
        me:host = host
        me:port = port
        me:clientId = "student_" + GenerateRandomId()
    end
    
    // Connection
    public action Connect() returns boolean
        // Code de connection
    end
    
    // Publication
    public action Publish(text topic, text message)
        // Code de publication
    end
    
    // Souscription
    public action Subscribe(text topic)
        // Code de souscription
    end
end
```

#### M5StackController
```quorum
class M5StackController
    // Propriétés privées
    private MQTTClient mqtt = undefined
    
    // Constructeur
    public action Create(MQTTClient client)
        me:mqtt = client
    end
    
    // Actions de base
    public action DisplayText(text message)
        mqtt:Publish("m5stack/display", message)
    end
    
    public action ClearScreen()
        mqtt:Publish("m5stack/display/clear", "")
    end
    
    // Contrôles LED
    public action SetLED(integer red, integer green, integer blue)
        text message = "{red:" + red + ",green:" + green + ",blue:" + blue + "}"
        mqtt:Publish("m5stack/led", message)
    end
end
```

#### HuskyLensController
```quorum
class HuskyLensController
    // Propriétés privées
    private MQTTClient mqtt = undefined
    private text currentMode = "FACE_RECOGNITION"
    
    // Constructeur
    public action Create(MQTTClient client)
        me:mqtt = client
    end
    
    // Modes de reconnaissance
    public action SetMode(text mode)
        if mode = "FACE_RECOGNITION" or 
           mode = "OBJECT_TRACKING" or
           mode = "LINE_TRACKING"
            me:currentMode = mode
            mqtt:Publish("huskylens/mode", mode)
        end
    end
    
    // Récupération des données
    public action GetObjects() returns Array<Object>
        mqtt:Publish("huskylens/request", "GET_OBJECTS")
        // Attendre et traiter la réponse
    end
end
```

## 3. Exemples d'utilisation

### 3.1 Connection basique
```quorum
use Libraries.Robotics.M5StackController
use Libraries.Robotics.HuskyLensController

class MyProgram
    action Main
        // Création du client MQTT
        MQTTClient client
        client:Create("localhost", 1883)
        client:Connect()
        
        // Initialisation des contrôleurs
        M5StackController m5
        m5:Create(client)
        
        HuskyLensController husky
        husky:Create(client)
        
        // Utilisation
        m5:DisplayText("Hello World!")
        husky:SetMode("FACE_RECOGNITION")
    end
end
```

### 3.2 Reconnaissance faciale
```quorum
class FaceDetector
    private HuskyLensController husky
    private M5StackController m5
    
    action Initialize
        MQTTClient client
        client:Create("localhost", 1883)
        client:Connect()
        
        husky:Create(client)
        m5:Create(client)
        
        husky:SetMode("FACE_RECOGNITION")
    end
    
    action DetectFaces
        Array<Object> faces = husky:GetObjects()
        
        if faces:GetSize() > 0
            m5:DisplayText("Detected " + faces:GetSize() + " faces!")
            m5:SetLED(0, 255, 0)  // LED verte
        else
            m5:DisplayText("No faces detected")
            m5:SetLED(255, 0, 0)  // LED rouge
        end
    end
end
```

## 4. Topics MQTT

### 4.1 Topics M5Stack
```yaml
m5stack/
├── display/
│   ├── text          # Affichage de texte
│   ├── clear         # Effacement écran
│   └── image         # Affichage image
├── led/
│   ├── set           # Configuration LED
│   └── clear         # Extinction LED
└── button/
    ├── a/pressed     # Bouton A pressé
    ├── b/pressed     # Bouton B pressé
    └── c/pressed     # Bouton C pressé
```

### 4.2 Topics HuskyLens
```yaml
huskylens/
├── mode/
│   ├── set           # Configuration mode
│   └── current       # Mode actuel
├── objects/
│   ├── request       # Demande données
│   └── response      # Réponse données
└── status/
    ├── connected     # État connection
    └── error         # Messages d'erreur
```

## 5. Format des messages

### 5.1 Messages M5Stack
```json
// Affichage texte
{
    "text": "Hello World",
    "x": 10,
    "y": 10,
    "size": 2,
    "color": "white"
}

// Configuration LED
{
    "red": 255,
    "green": 0,
    "blue": 0
}
```

### 5.2 Messages HuskyLens
```json
// Réponse objets détectés
{
    "type": "FACE_RECOGNITION",
    "objects": [
        {
            "id": 1,
            "x": 120,
            "y": 80,
            "width": 40,
            "height": 40,
            "confidence": 0.95
        }
    ]
}
```

## 6. Gestion des erreurs

### 6.1 Types d'erreurs
```quorum
class MQTTError
    public constant text CONNECTION_ERROR = "CONNECTION_ERROR"
    public constant text TIMEOUT_ERROR = "TIMEOUT_ERROR"
    public constant text PUBLISH_ERROR = "PUBLISH_ERROR"
    public constant text SUBSCRIBE_ERROR = "SUBSCRIBE_ERROR"
end
```

### 6.2 Exemple de gestion
```quorum
action HandleError(text errorType, text message)
    if errorType = MQTTError:CONNECTION_ERROR
        output "Erreur de connexion: " + message
    elseif errorType = MQTTError:TIMEOUT_ERROR
        output "Timeout: " + message
    end
    
    // Log de l'erreur
    LogError(errorType, message)
end
```

## 7. Tests unitaires

### 7.1 Tests de connection
```quorum
class MQTTClientTest
    action TestConnection
        MQTTClient client
        client:Create("localhost", 1883)
        
        boolean result = client:Connect()
        assert result = true
    end
end
```

### 7.2 Tests de contrôle
```quorum
class M5StackTest
    action TestDisplay
        MQTTClient client
        client:Create("localhost", 1883)
        client:Connect()
        
        M5StackController m5
        m5:Create(client)
        
        m5:DisplayText("Test")
        // Vérifier que le texte est affiché
    end
end
```

## 8. Documentation des erreurs communes

### 8.1 Problèmes de connexion
1. Vérifier que le broker MQTT est accessible
2. Vérifier les paramètres de connexion
3. Vérifier le pare-feu

### 8.2 Problèmes de commandes
1. Vérifier le format des messages
2. Vérifier les topics utilisés
3. Vérifier les droits d'accès

## 9. Limitations connues

1. Pas de support SSL/TLS
2. Limitation à un seul broker
3. Pas de gestion de la qualité de service (QoS)
4. Timeout fixe de 5 secondes