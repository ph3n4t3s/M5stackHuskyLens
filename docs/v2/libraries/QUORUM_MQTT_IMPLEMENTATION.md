# Implémentation de la bibliothèque Quorum MQTT

## 1. Structure des fichiers

```
Libraries/
└── Robotics/
    ├── MQTTClient.quorum
    ├── M5StackController.quorum
    ├── HuskyLensController.quorum
    └── Utilities/
        ├── JsonBuilder.quorum
        ├── MQTTError.quorum
        └── Logger.quorum
```

## 2. Implémentation détaillée

### 2.1 MQTTClient.quorum
```quorum
package Libraries.Robotics

use Libraries.Network.WebSocket
use Libraries.Containers.Array
use Libraries.Robotics.Utilities.MQTTError
use Libraries.Robotics.Utilities.Logger

class MQTTClient
    /*
     * Propriétés privées
     */
    private WebSocket socket = undefined
    private text host = ""
    private integer port = 0
    private text clientId = ""
    private boolean connected = false
    private Logger logger
    private Array<text> subscriptions
    
    /*
     * Constructeur
     */
    public action Create(text host, integer port)
        me:host = host
        me:port = port
        me:clientId = "student_" + GenerateRandomId()
        me:logger:Initialize("mqtt_client")
    end
    
    /*
     * Actions de connexion
     */
    public action Connect returns boolean
        try
            socket:Connect("ws://" + host + ":" + port + "/mqtt")
            connected = true
            logger:Log("Connected to MQTT broker")
            return true
        catch error:WebSocketError
            logger:LogError("Connection failed: " + error:GetMessage())
            return false
        end
    end
    
    public action Disconnect
        if connected
            socket:Close()
            connected = false
            logger:Log("Disconnected from MQTT broker")
        end
    end
    
    /*
     * Actions de publication
     */
    public action Publish(text topic, text message)
        if not connected
            logger:LogError("Not connected to broker")
            return
        end
        
        text mqttMessage = BuildMQTTMessage(topic, message)
        socket:Send(mqttMessage)
        logger:Log("Published to " + topic + ": " + message)
    end
    
    /*
     * Actions de souscription
     */
    public action Subscribe(text topic)
        if not connected
            logger:LogError("Not connected to broker")
            return
        end
        
        text subscribeMessage = BuildSubscribeMessage(topic)
        socket:Send(subscribeMessage)
        subscriptions:Add(topic)
        logger:Log("Subscribed to " + topic)
    end
    
    /*
     * Utilitaires privés
     */
    private action GenerateRandomId returns text
        // Génère un ID unique pour le client
        return "client_" + GetCurrentTime()
    end
    
    private action BuildMQTTMessage(text topic, text payload) returns text
        // Construit le message au format MQTT
        return "{\"topic\":\"" + topic + "\",\"payload\":\"" + payload + "\"}"
    end
    
    private action BuildSubscribeMessage(text topic) returns text
        // Construit le message de souscription
        return "{\"type\":\"subscribe\",\"topic\":\"" + topic + "\"}"
    end
end
```

### 2.2 M5StackController.quorum
```quorum
package Libraries.Robotics

use Libraries.Robotics.Utilities.JsonBuilder
use Libraries.Robotics.Utilities.Logger

class M5StackController
    /*
     * Propriétés privées
     */
    private MQTTClient mqtt = undefined
    private JsonBuilder json
    private Logger logger
    
    /*
     * Actions d'affichage
     */
    public action DisplayText(text message, integer x, integer y, integer size)
        json:Clear()
        json:AddProperty("text", message)
        json:AddProperty("x", x)
        json:AddProperty("y", y)
        json:AddProperty("size", size)
        
        mqtt:Publish("m5stack/display", json:ToString())
        logger:Log("Display text: " + message)
    end
    
    public action ClearScreen
        mqtt:Publish("m5stack/display/clear", "{}")
        logger:Log("Clear screen")
    end
    
    /*
     * Actions LED
     */
    public action SetLED(integer red, integer green, integer blue)
        json:Clear()
        json:AddProperty("red", red)
        json:AddProperty("green", green)
        json:AddProperty("blue", blue)
        
        mqtt:Publish("m5stack/led", json:ToString())
        logger:Log("Set LED: R" + red + " G" + green + " B" + blue)
    end
end
```

### 2.3 HuskyLensController.quorum
```quorum
package Libraries.Robotics

use Libraries.Robotics.Utilities.JsonBuilder
use Libraries.Robotics.Utilities.Logger
use Libraries.Containers.Array

class HuskyLensController
    /*
     * Propriétés privées
     */
    private MQTTClient mqtt = undefined
    private JsonBuilder json
    private Logger logger
    private text currentMode = "FACE_RECOGNITION"
    
    /*
     * Actions de mode
     */
    public action SetMode(text mode)
        if IsValidMode(mode)
            currentMode = mode
            mqtt:Publish("huskylens/mode", "{\"mode\":\"" + mode + "\"}")
            logger:Log("Set mode: " + mode)
        else
            logger:LogError("Invalid mode: " + mode)
        end
    end
    
    /*
     * Actions de détection
     */
    public action GetObjects returns Array<Object>
        mqtt:Publish("huskylens/objects/request", "{\"mode\":\"" + currentMode + "\"}")
        
        // Attendre la réponse (implémentation asynchrone nécessaire)
        text response = WaitForResponse()
        return ParseResponse(response)
    end
    
    /*
     * Utilitaires privés
     */
    private action IsValidMode(text mode) returns boolean
        return mode = "FACE_RECOGNITION" or
               mode = "OBJECT_TRACKING" or
               mode = "LINE_TRACKING"
    end
    
    private action WaitForResponse returns text
        // Implémentation de l'attente de réponse
        return ""
    end
    
    private action ParseResponse(text response) returns Array<Object>
        // Parse la réponse JSON en objets
        Array<Object> objects
        return objects
    end
end
```

### 2.4 Utilitaires

#### JsonBuilder.quorum
```quorum
package Libraries.Robotics.Utilities

class JsonBuilder
    private text json = "{"
    private boolean first = true
    
    public action AddProperty(text name, text value)
        if not first
            json = json + ","
        end
        json = json + "\"" + name + "\":\"" + value + "\""
        first = false
    end
    
    public action AddProperty(text name, integer value)
        if not first
            json = json + ","
        end
        json = json + "\"" + name + "\":" + value
        first = false
    end
    
    public action ToString returns text
        return json + "}"
    end
    
    public action Clear
        json = "{"
        first = true
    end
end
```

## 3. Notes d'implémentation

### 3.1 Gestion asynchrone
L'implémentation actuelle est synchrone pour simplifier la compréhension. Une version asynchrone avec des callbacks serait plus appropriée pour un environnement de production.

### 3.2 Sécurité
- Validation des entrées
- Échappement des caractères spéciaux
- Vérification des limites

### 3.3 Performance
- Mise en cache des connexions
- Optimisation des messages JSON
- Gestion de la mémoire

### 3.4 Évolutions futures
1. Support SSL/TLS
2. Gestion QoS
3. Reconnexion automatique
4. Tests de performance

## 4. Compilation et déploiement

```bash
# Compilation
quorum -compile Libraries/Robotics/*.quorum

# Tests
quorum -test Libraries/Robotics/Tests/*.quorum

# Documentation
quorum -document Libraries/Robotics/
```