# Spécification API - M5Stack HuskyLens V2.0

## 1. Vue d'ensemble de l'API

L'API REST fournit les points de terminaison nécessaires pour :
- La gestion des utilisateurs
- La soumission et l'exécution du code
- La gestion des missions
- Le monitoring du système
- L'administration de la plateforme

### 1.1 Format des réponses
```json
{
    "success": true|false,
    "data": {},
    "error": {
        "code": "ERROR_CODE",
        "message": "Error description"
    }
}
```

## 2. Points de terminaison

### 2.1 Authentification
```
POST /api/auth/login
GET  /api/auth/logout
POST /api/auth/refresh
GET  /api/auth/status
```

#### Exemple de login
```json
// POST /api/auth/login
{
    "username": "student1",
    "password": "password123"
}

// Réponse
{
    "success": true,
    "data": {
        "token": "jwt_token_here",
        "user": {
            "id": 1,
            "username": "student1",
            "role": "student"
        }
    }
}
```

### 2.2 Gestion des missions
```
GET    /api/missions
GET    /api/missions/{id}
POST   /api/missions/{id}/submit
GET    /api/missions/{id}/status
GET    /api/missions/{id}/queue
DELETE /api/missions/{id}/cancel
```

#### Exemple de soumission
```json
// POST /api/missions/1/submit
{
    "code": "use HuskyLens\nuse M5Stack\n...",
    "language": "quorum"
}

// Réponse
{
    "success": true,
    "data": {
        "submissionId": "abc123",
        "queuePosition": 2,
        "estimatedWait": "30s"
    }
}
```

### 2.3 Administration
```
GET    /api/admin/users
POST   /api/admin/users
PUT    /api/admin/users/{id}
DELETE /api/admin/users/{id}
GET    /api/admin/system/status
POST   /api/admin/system/reset
PUT    /api/admin/config
```

#### Exemple de configuration
```json
// PUT /api/admin/config
{
    "missionTimeLimit": 300,
    "queueTimeout": 60,
    "maxAttempts": 3
}

// Réponse
{
    "success": true,
    "data": {
        "updated": ["missionTimeLimit", "queueTimeout", "maxAttempts"]
    }
}
```

### 2.4 Monitoring
```
GET /api/monitor/queue
GET /api/monitor/active
GET /api/monitor/stats
GET /api/monitor/logs
```

#### Exemple de statistiques
```json
// GET /api/monitor/stats
{
    "success": true,
    "data": {
        "activeMission": 1,
        "activeUsers": 25,
        "queueLength": 3,
        "completions": {
            "total": 45,
            "byMission": {
                "1": 15,
                "2": 20,
                "3": 10
            }
        }
    }
}
```

### 2.5 Monitoring détaillé
```
GET /api/monitor/system
GET /api/monitor/system/metrics
GET /api/monitor/system/health
GET /api/monitor/system/alerts
GET /api/monitor/m5stack/status
GET /api/monitor/m5stack/metrics
GET /api/monitor/missions/metrics
```

#### Exemple de monitoring système
```json
// GET /api/monitor/system
{
    "success": true,
    "data": {
        "system": {
            "cpu": {
                "usage": 45.2,
                "cores": 4,
                "temperature": 52.3
            },
            "memory": {
                "total": 8192,
                "used": 4096,
                "free": 4096
            },
            "disk": {
                "total": 50000,
                "used": 25000,
                "free": 25000
            }
        },
        "docker": {
            "containers": {
                "total": 6,
                "running": 6,
                "stopped": 0
            },
            "images": 8,
            "volumes": 4
        }
    }
}
```

#### Exemple de métriques M5Stack
```json
// GET /api/monitor/m5stack/metrics
{
    "success": true,
    "data": {
        "connection": {
            "status": "connected",
            "uptime": "2h 15m",
            "reconnects": 0
        },
        "performance": {
            "average_execution_time": 1.2,
            "max_execution_time": 3.5,
            "success_rate": 98.5
        },
        "queue": {
            "current_length": 3,
            "average_wait_time": 45,
            "processed_last_hour": 120
        }
    }
}
```

### 2.6 WebSocket Events
```javascript
// Connection
socket.on('connect', () => {
    console.log('Connected to WebSocket');
});

// Mission updates
socket.on('mission:update', (data) => {
    console.log('Mission update:', data);
});

// Queue updates
socket.on('queue:update', (data) => {
    console.log('Queue update:', data);
});

// System status
socket.on('system:status', (data) => {
    console.log('System status:', data);
});
```

## 3. Sécurité

### 3.1 Authentification
- Utilisation de JWT (JSON Web Tokens)
- Expiration des tokens après 1 heure
- Refresh tokens pour renouvellement automatique

### 3.2 Autorisations
```javascript
// Middleware de vérification des rôles
const checkRole = (role) => async (req, res, next) => {
    try {
        const user = req.user;
        if (user.role !== role) {
            throw new Error('Unauthorized');
        }
        next();
    } catch (error) {
        res.status(403).json({
            success: false,
            error: {
                code: 'FORBIDDEN',
                message: 'Access denied'
            }
        });
    }
};
```

### 3.3 Rate Limiting
```javascript
const rateLimit = require('express-rate-limit');

const limiter = rateLimit({
    windowMs: 15 * 60 * 1000, // 15 minutes
    max: 100 // limite par IP
});

app.use('/api/', limiter);
```

## 4. Gestion des erreurs

### 4.1 Codes d'erreur
```javascript
const ErrorCodes = {
    AUTH_FAILED: 'Authentication failed',
    INVALID_INPUT: 'Invalid input data',
    MISSION_NOT_FOUND: 'Mission not found',
    QUEUE_FULL: 'Queue is full',
    EXECUTION_TIMEOUT: 'Execution timeout',
    SYSTEM_ERROR: 'System error'
};
```

### 4.2 Format des erreurs
```json
{
    "success": false,
    "error": {
        "code": "QUEUE_FULL",
        "message": "The execution queue is currently full. Please try again later.",
        "details": {
            "queueLength": 10,
            "maxQueue": 10,
            "estimatedWait": "5m"
        }
    }
}
```

## 5. Documentation Swagger

```yaml
openapi: 3.0.0
info:
  title: M5Stack HuskyLens Game API
  version: 2.0.0
  description: API for M5Stack HuskyLens educational game platform

paths:
  /api/auth/login:
    post:
      summary: Authenticate user
      tags: [Auth]
      requestBody:
        required: true
        content:
          application/json:
            schema:
              type: object
              properties:
                username:
                  type: string
                password:
                  type: string
      responses:
        '200':
          description: Login successful
          content:
            application/json:
              schema:
                type: object
                properties:
                  success:
                    type: boolean
                  data:
                    type: object
                    properties:
                      token:
                        type: string
                      user:
                        type: object
```