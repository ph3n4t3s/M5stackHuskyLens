# Flux de jeu Node-RED - M5Stack HuskyLens V2.0

## 1. Flux de mission

### 1.1 Démarrage de mission
```json
[
    {
        "id": "mission_start",
        "type": "inject",
        "name": "Start Mission",
        "props": [
            {
                "p": "payload",
                "v": "{\"type\": \"start_mission\", \"mission_id\": 1}",
                "vt": "json"
            }
        ]
    },
    {
        "id": "mission_init",
        "type": "function",
        "name": "Initialize Mission",
        "func": `
            const mission = global.get('missions').find(m => 
                m.id === msg.payload.mission_id
            );
            
            if (!mission) {
                return {error: 'Mission not found'};
            }
            
            global.set('current_mission', {
                ...mission,
                startTime: Date.now(),
                participants: []
            });
            
            return {
                payload: {
                    type: 'mission_started',
                    mission: mission
                }
            };
        `
    },
    {
        "id": "broadcast_mission",
        "type": "mqtt out",
        "topic": "game/mission/current",
        "qos": "2"
    }
]
```

### 1.2 Validation de soumission
```json
[
    {
        "id": "submission_receiver",
        "type": "mqtt in",
        "topic": "game/submission/+",
        "qos": "2"
    },
    {
        "id": "submission_validator",
        "type": "function",
        "name": "Validate Submission",
        "func": `
            const currentMission = global.get('current_mission');
            const submission = msg.payload;
            
            // Vérification de la mission active
            if (submission.mission_id !== currentMission.id) {
                return {error: 'Invalid mission'};
            }
            
            // Validation du code
            const result = validateQuorumCode(
                submission.code,
                currentMission.requirements
            );
            
            return {
                payload: {
                    submission_id: submission.id,
                    valid: result.valid,
                    points: calculatePoints(result),
                    details: result.details
                }
            };
        `
    },
    {
        "id": "update_scores",
        "type": "function",
        "name": "Update Scores",
        "func": `
            const submission = msg.payload;
            const scores = global.get('scores') || {};
            
            scores[submission.user_id] = 
                (scores[submission.user_id] || 0) + submission.points;
            
            global.set('scores', scores);
            
            return {
                payload: {
                    type: 'score_update',
                    user_id: submission.user_id,
                    points: submission.points,
                    total: scores[submission.user_id]
                }
            };
        `
    }
]
```

## 2. Flux de visualisation

### 2.1 Mise à jour du tableau de bord
```json
[
    {
        "id": "dashboard_updater",
        "type": "function",
        "name": "Update Dashboard",
        "func": `
            const current_mission = global.get('current_mission');
            const scores = global.get('scores') || {};
            
            // Préparation du classement
            const leaderboard = Object.entries(scores)
                .map(([id, points]) => ({id, points}))
                .sort((a, b) => b.points - a.points)
                .slice(0, 10);
            
            // Statistiques de la mission
            const mission_stats = {
                completed: global.get('mission_completed') || 0,
                attempts: global.get('mission_attempts') || 0,
                averageTime: calculateAverageTime()
            };
            
            return {
                payload: {
                    mission: current_mission,
                    leaderboard: leaderboard,
                    stats: mission_stats
                }
            };
        `
    },
    {
        "id": "dashboard_broadcast",
        "type": "websocket out",
        "path": "/ws/dashboard"
    }
]
```

### 2.2 Animation d'événements
```json
[
    {
        "id": "event_handler",
        "type": "function",
        "name": "Handle Events",
        "func": `
            const events = {
                MISSION_COMPLETE: (data) => ({
                    type: 'celebration',
                    duration: 5000,
                    title: 'Mission Accomplie !',
                    content: \`\${data.user} a complété la mission !\`
                }),
                HIGH_SCORE: (data) => ({
                    type: 'achievement',
                    duration: 3000,
                    title: 'Nouveau Record !',
                    content: \`\${data.user} a établi un nouveau record !\`
                })
            };
            
            const event = events[msg.payload.type](msg.payload.data);
            return {payload: event};
        `
    },
    {
        "id": "animation_controller",
        "type": "function",
        "name": "Control Animations",
        "func": `
            const animations = {
                celebration: () => ({
                    css: 'celebration-animation',
                    sound: '/sounds/success.mp3'
                }),
                achievement: () => ({
                    css: 'achievement-animation',
                    sound: '/sounds/achievement.mp3'
                })
            };
            
            const event = msg.payload;
            const animation = animations[event.type]();
            
            return {
                payload: {
                    ...event,
                    animation
                }
            };
        `
    }
]
```

## 3. Flux M5Stack

### 3.1 Communication série
```json
[
    {
        "id": "serial_receiver",
        "type": "serial in",
        "port": "/dev/ttyUSB0",
        "baudRate": "115200"
    },
    {
        "id": "serial_parser",
        "type": "function",
        "name": "Parse Serial Data",
        "func": `
            try {
                const data = JSON.parse(msg.payload);
                
                // Validation du format
                if (!data.type || !data.content) {
                    throw new Error('Invalid data format');
                }
                
                return {
                    payload: {
                        type: data.type,
                        content: data.content,
                        timestamp: Date.now()
                    }
                };
            } catch (error) {
                node.error('Parse error: ' + error.message);
                return null;
            }
        `
    }
]
```

### 3.2 Contrôle HuskyLens
```json
[
    {
        "id": "huskylens_controller",
        "type": "function",
        "name": "Control HuskyLens",
        "func": `
            const commands = {
                SET_MODE: (mode) => ({
                    command: 0x2A,
                    params: [mode]
                }),
                TAKE_PHOTO: () => ({
                    command: 0x30,
                    params: []
                }),
                GET_OBJECTS: () => ({
                    command: 0x2B,
                    params: []
                })
            };
            
            const cmd = msg.payload.command;
            const params = msg.payload.params;
            
            if (!commands[cmd]) {
                return {error: 'Invalid command'};
            }
            
            return {
                payload: commands[cmd](params)
            };
        `
    },
    {
        "id": "huskylens_response",
        "type": "function",
        "name": "Process Response",
        "func": `
            const parseResponse = (data) => {
                switch (data.type) {
                    case 'objects':
                        return processObjects(data.content);
                    case 'face':
                        return processFaces(data.content);
                    case 'line':
                        return processLines(data.content);
                    default:
                        return data;
                }
            };
            
            return {
                payload: parseResponse(msg.payload)
            };
        `
    }
]
```

## 4. Flux de gestion des erreurs

### 4.1 Gestion des exceptions
```json
[
    {
        "id": "error_handler",
        "type": "catch",
        "scope": ["serial_parser", "huskylens_controller"],
        "func": `
            const error = msg.error;
            
            // Log de l'erreur
            console.error('Error:', error);
            
            // Notification
            const notification = {
                type: 'error',
                title: 'Erreur système',
                message: error.message,
                timestamp: Date.now()
            };
            
            // Broadcast aux admins
            node.send([
                {payload: notification, topic: 'admin/notifications'},
                {payload: error, topic: 'system/errors'}
            ]);
        `
    }
]
```

### 4.2 Récupération automatique
```json
[
    {
        "id": "recovery_manager",
        "type": "function",
        "name": "System Recovery",
        "func": `
            const recovery = {
                SERIAL_ERROR: async () => {
                    // Tentative de reconnexion série
                    await disconnectSerial();
                    await delay(1000);
                    return connectSerial();
                },
                
                MQTT_ERROR: async () => {
                    // Reconnexion MQTT
                    await disconnectMQTT();
                    await delay(1000);
                    return connectMQTT();
                },
                
                MISSION_ERROR: async () => {
                    // Reset de la mission actuelle
                    const mission = global.get('current_mission');
                    return resetMission(mission.id);
                }
            };
            
            const error = msg.payload;
            const recover = recovery[error.type];
            
            if (recover) {
                try {
                    await recover();
                    return {payload: {recovered: true}};
                } catch (err) {
                    return {payload: {recovered: false, error: err}};
                }
            }
        `
    }
]
```

## 5. Flux de monitoring

### 5.1 Collecte de métriques
```json
[
    {
        "id": "metrics_collector",
        "type": "function",
        "name": "Collect Metrics",
        "func": `
            const metrics = {
                system: {
                    cpu: process.cpuUsage(),
                    memory: process.memoryUsage(),
                    uptime: process.uptime()
                },
                
                game: {
                    activePlayers: global.get('active_players') || 0,
                    currentMission: global.get('current_mission'),
                    completedMissions: global.get('completed_missions') || 0
                },
                
                hardware: {
                    m5stack: global.get('m5stack_status'),
                    huskylens: global.get('huskylens_status')
                }
            };
            
            return {payload: metrics};
        `
    }
]
```

### 5.2 Alertes système
```json
[
    {
        "id": "alert_manager",
        "type": "function",
        "name": "Manage Alerts",
        "func": `
            const checkThresholds = (metrics) => {
                const alerts = [];
                
                // Vérification CPU
                if (metrics.system.cpu > 80) {
                    alerts.push({
                        type: 'warning',
                        message: 'CPU usage high',
                        value: metrics.system.cpu
                    });
                }
                
                // Vérification mémoire
                if (metrics.system.memory.heapUsed > 
                    metrics.system.memory.heapTotal * 0.9) {
                    alerts.push({
                        type: 'critical',
                        message: 'Memory usage critical',
                        value: metrics.system.memory.heapUsed
                    });
                }
                
                // Vérification matériel
                if (!metrics.hardware.m5stack.connected) {
                    alerts.push({
                        type: 'error',
                        message: 'M5Stack disconnected'
                    });
                }
                
                return alerts;
            };
            
            const alerts = checkThresholds(msg.payload);
            return {payload: alerts};
        `
    }
]
```