# Configuration Node-RED - M5Stack HuskyLens V2.0

## 1. Configuration système

### 1.1 Structure des dossiers
```
nodered/
├── settings.js          # Configuration Node-RED
├── flows.json          # Définition des flux
├── flows_cred.json    # Données d'identification cryptées
├── package.json       # Dépendances
└── data/              # Données persistantes
    ├── missions/      # Configuration des missions
    ├── templates/     # Templates de messages
    └── dashboards/    # Configuration des tableaux de bord
```

### 1.2 Configuration système (settings.js)
```javascript
module.exports = {
    // Configuration du runtime
    uiPort: process.env.PORT || 1880,
    mqttReconnectTime: 15000,
    serialReconnectTime: 15000,
    debugMaxLength: 1000,

    // Sécurité
    adminAuth: {
        type: "credentials",
        users: [{
            username: "admin",
            password: "$2a$08$...",
            permissions: "*"
        }]
    },

    // Stockage des flux
    flowFile: 'flows.json',
    flowFilePretty: true,
    
    // Configuration des nœuds
    functionGlobalContext: {
        // Variables globales personnalisées
        gameConfig: require('./data/config.json'),
        missionsData: require('./data/missions.json')
    },
    
    // Configuration du journal
    logging: {
        console: {
            level: "info",
            metrics: false,
            audit: false
        }
    },
    
    // Configuration MQTT
    mqtt: {
        broker: 'localhost',
        port: 1883,
        username: 'nodered',
        password: 'nodered_secret'
    },
    
    // Configuration du tableau de bord
    ui: {
        path: "dashboard",
        middleware: require('./middleware/auth'),
        theme: {
            name: "theme-light",
            custom: true
        }
    }
};
```

## 2. Nœuds personnalisés

### 2.1 M5Stack Node
```javascript
module.exports = function(RED) {
    function M5StackNode(config) {
        RED.nodes.createNode(this, config);
        const node = this;
        
        // Configuration
        this.serialPort = config.serialPort;
        this.baudRate = config.baudRate;
        
        // État
        this.connected = false;
        this.status({fill:"red", shape:"ring", text:"disconnected"});
        
        // Gestionnaire de messages
        this.on('input', function(msg) {
            try {
                const command = msg.payload;
                sendToM5Stack(command, (response) => {
                    msg.payload = response;
                    node.send(msg);
                });
            } catch(error) {
                node.error(error);
            }
        });
        
        // Nettoyage
        this.on('close', function() {
            disconnect();
        });
    }
    
    RED.nodes.registerType("m5stack", M5StackNode);
}
```

### 2.2 Mission Validator Node
```javascript
module.exports = function(RED) {
    function MissionValidatorNode(config) {
        RED.nodes.createNode(this, config);
        const node = this;
        
        // Configuration
        this.missionId = config.missionId;
        this.requirements = config.requirements;
        
        // Validation
        this.on('input', function(msg) {
            try {
                const result = validateMission(
                    msg.payload,
                    this.requirements
                );
                
                msg.payload = {
                    valid: result.valid,
                    score: result.score,
                    details: result.details
                };
                
                node.send([msg, null]);
            } catch(error) {
                msg.payload = {error: error.message};
                node.send([null, msg]);
            }
        });
    }
    
    RED.nodes.registerType("mission-validator", MissionValidatorNode);
}
```

## 3. Configuration des tableaux de bord

### 3.1 Dashboard administrateur
```json
{
    "name": "Admin Dashboard",
    "theme": "dark",
    "pages": [
        {
            "title": "Vue d'ensemble",
            "groups": [
                {
                    "name": "System Status",
                    "width": "6",
                    "height": "4",
                    "widgets": [
                        {
                            "type": "gauge",
                            "name": "CPU Usage",
                            "topic": "system/cpu"
                        },
                        {
                            "type": "chart",
                            "name": "Memory",
                            "topic": "system/memory"
                        }
                    ]
                },
                {
                    "name": "Mission Control",
                    "width": "6",
                    "height": "4",
                    "widgets": [
                        {
                            "type": "dropdown",
                            "name": "Current Mission",
                            "topic": "mission/current"
                        },
                        {
                            "type": "button",
                            "name": "Next Mission",
                            "topic": "mission/next"
                        }
                    ]
                }
            ]
        }
    ]
}
```

### 3.2 Dashboard affichage
```json
{
    "name": "Display Dashboard",
    "theme": "light",
    "pages": [
        {
            "title": "Game Display",
            "groups": [
                {
                    "name": "Leaderboard",
                    "width": "4",
                    "height": "6",
                    "widgets": [
                        {
                            "type": "template",
                            "name": "Top Players",
                            "template": "<div class='leaderboard'>...</div>"
                        }
                    ]
                },
                {
                    "name": "Current Mission",
                    "width": "8",
                    "height": "6",
                    "widgets": [
                        {
                            "type": "template",
                            "name": "Mission Details",
                            "template": "<div class='mission'>...</div>"
                        }
                    ]
                }
            ]
        }
    ]
}
```

## 4. Configuration MQTT

### 4.1 Configuration broker
```javascript
// mosquitto.conf
listener 1883
protocol mqtt

listener 9001
protocol websockets

persistence true
persistence_location /mosquitto/data

password_file /mosquitto/config/passwd
allow_anonymous false

log_dest file /mosquitto/log/mosquitto.log
log_type all
```

### 4.2 Structure des topics
```javascript
const topics = {
    system: {
        status: 'system/status',
        health: 'system/health/#',
        config: 'system/config/#'
    },
    
    game: {
        mission: 'game/mission/#',
        score: 'game/score/#',
        events: 'game/events/#'
    },
    
    m5stack: {
        command: 'm5stack/command',
        response: 'm5stack/response',
        status: 'm5stack/status'
    },
    
    display: {
        update: 'display/update',
        control: 'display/control'
    }
};
```

## 5. Middleware

### 5.1 Authentification
```javascript
// middleware/auth.js
module.exports = function(req, res, next) {
    // Vérification du token JWT
    const token = req.headers.authorization;
    if (!token) {
        return res.status(401).json({error: 'No token provided'});
    }
    
    try {
        const decoded = jwt.verify(token, process.env.JWT_SECRET);
        req.user = decoded;
        next();
    } catch(error) {
        res.status(401).json({error: 'Invalid token'});
    }
};
```

### 5.2 Rate Limiting
```javascript
// middleware/rateLimit.js
const rateLimit = require('express-rate-limit');

module.exports = rateLimit({
    windowMs: 15 * 60 * 1000, // 15 minutes
    max: 100, // limit each IP to 100 requests per windowMs
    message: 'Too many requests from this IP'
});
```

## 6. Monitoring

### 6.1 Métriques système
```javascript
// monitoring/metrics.js
module.exports = {
    collectMetrics: function() {
        return {
            system: {
                cpu: process.cpuUsage(),
                memory: process.memoryUsage(),
                uptime: process.uptime()
            },
            
            nodered: {
                flows: RED.nodes.getFlows().length,
                nodes: RED.nodes.getNodeList().length,
                messages: messageCount
            },
            
            mqtt: {
                connections: mqttStats.connections,
                messages: mqttStats.messages,
                bytes: mqttStats.bytes
            }
        };
    },
    
    startCollection: function(interval = 10000) {
        setInterval(() => {
            const metrics = this.collectMetrics();
            RED.nodes.emit('metrics', metrics);
        }, interval);
    }
};
```

### 6.2 Journalisation
```javascript
// monitoring/logger.js
const winston = require('winston');

module.exports = winston.createLogger({
    level: 'info',
    format: winston.format.json(),
    transports: [
        new winston.transports.File({
            filename: 'error.log',
            level: 'error'
        }),
        new winston.transports.File({
            filename: 'combined.log'
        })
    ]
});
```