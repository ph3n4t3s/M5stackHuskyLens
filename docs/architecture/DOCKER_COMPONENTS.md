# Composants Docker - M5Stack HuskyLens V2.0

## 1. Structure Docker Compose

### 1.1 docker-compose.yml
```yaml
version: '3.8'

services:
  # Interface Web principale
  webapp:
    build:
      context: ./webapp
      dockerfile: Dockerfile
    ports:
      - "80:80"
    volumes:
      - ./webapp:/app
      - ./logs/webapp:/var/log/nginx
    environment:
      - NODE_ENV=production
      - API_URL=http://api:3000
      - MQTT_WS_URL=ws://mqtt:9001
    depends_on:
      - api
      - mqtt
    networks:
      - frontend
      - backend
    restart: unless-stopped

  # API Backend
  api:
    build:
      context: ./api
      dockerfile: Dockerfile
    ports:
      - "3000:3000"
    volumes:
      - ./api:/app
      - ./logs/api:/app/logs
    environment:
      - NODE_ENV=production
      - DB_HOST=db
      - DB_PORT=5432
      - MQTT_HOST=mqtt
      - MQTT_PORT=1883
    depends_on:
      - db
      - mqtt
    networks:
      - backend
    restart: unless-stopped

  # Broker MQTT
  mqtt:
    image: eclipse-mosquitto:latest
    ports:
      - "1883:1883"  # MQTT
      - "9001:9001"  # MQTT over WebSocket
    volumes:
      - ./config/mqtt/mosquitto.conf:/mosquitto/config/mosquitto.conf
      - ./config/mqtt/password_file:/mosquitto/config/password_file
      - ./data/mqtt:/mosquitto/data
      - ./logs/mqtt:/mosquitto/log
    networks:
      - backend
    restart: unless-stopped

  # Base de données
  db:
    image: postgres:14-alpine
    ports:
      - "5432:5432"
    volumes:
      - ./data/db:/var/lib/postgresql/data
      - ./config/db/init.sql:/docker-entrypoint-initdb.d/init.sql
    environment:
      - POSTGRES_USER=m5stack
      - POSTGRES_PASSWORD=m5stack_secret
      - POSTGRES_DB=m5stack_game
    networks:
      - backend
    restart: unless-stopped

  # Node-RED
  nodered:
    image: nodered/node-red:latest
    ports:
      - "1880:1880"
    volumes:
      - ./data/nodered:/data
    environment:
      - NODE_RED_ENABLE_PROJECTS=true
      - NODE_RED_ENABLE_TOURS=false
    networks:
      - backend
    depends_on:
      - mqtt
    restart: unless-stopped

  # Gestionnaire M5Stack
  m5stack:
    build:
      context: ./m5stack-manager
      dockerfile: Dockerfile
    volumes:
      - ./m5stack-manager:/app
      - ./logs/m5stack:/app/logs
    devices:
      - "/dev/ttyUSB0:/dev/ttyUSB0"
    environment:
      - MQTT_HOST=mqtt
      - MQTT_PORT=1883
      - SERIAL_PORT=/dev/ttyUSB0
    depends_on:
      - mqtt
    networks:
      - backend
    restart: unless-stopped

networks:
  frontend:
    driver: bridge
  backend:
    driver: bridge

volumes:
  db_data:
    driver: local
    driver_opts:
      type: none
      o: bind
      device: ${PWD}/data/db
  
  nodered_data:
    driver: local
    driver_opts:
      type: none
      o: bind
      device: ${PWD}/data/nodered
      
  m5stack_data:
    driver: local
    driver_opts:
      type: none
      o: bind
      device: ${PWD}/data/m5stack
      
  log_data:
    driver: local
    driver_opts:
      type: none
      o: bind
      device: ${PWD}/logs
```

## 2. Configurations des Services

### 2.1 WebApp (Nginx)
```nginx
# nginx.conf
server {
    listen 80;
    server_name _;

    root /app/public;
    index index.html;

    # Configuration pour l'interface étudiant
    location /student {
        try_files $uri $uri/ /student/index.html;
    }

    # Configuration pour l'interface admin
    location /admin {
        try_files $uri $uri/ /admin/index.html;
    }

    # Configuration pour l'interface d'affichage
    location /display {
        try_files $uri $uri/ /display/index.html;
    }

    # API Proxy
    location /api {
        proxy_pass http://api:3000;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection 'upgrade';
        proxy_set_header Host $host;
        proxy_cache_bypass $http_upgrade;
    }

    # WebSocket MQTT
    location /mqtt {
        proxy_pass http://mqtt:9001;
        proxy_http_version 1.1;
        proxy_set_header Upgrade $http_upgrade;
        proxy_set_header Connection "Upgrade";
        proxy_set_header Host $host;
    }
}
```

### 2.2 MQTT (Mosquitto)
```conf
# mosquitto.conf
listener 1883
protocol mqtt

listener 9001
protocol websockets

persistence true
persistence_location /mosquitto/data/

log_dest file /mosquitto/log/mosquitto.log
log_dest stdout

password_file /mosquitto/config/password_file
allow_anonymous false
```

### 2.3 Node-RED
```json
{
    "editor": {
        "theme": "midnight-red"
    },
    "flowFile": "flows.json",
    "flowFilePretty": true,
    "uiPort": 1880,
    "mqtt": {
        "broker": "mqtt",
        "port": 1883,
        "username": "nodered",
        "password": "nodered_secret"
    }
}
```

### 2.4 Base de données
```sql
-- init.sql
-- Création des tables de base
CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    display_name VARCHAR(100),
    role VARCHAR(20) NOT NULL DEFAULT 'student',
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
);

-- Insert default admin user
INSERT INTO users (username, password_hash, display_name, role)
VALUES ('admin', '$2b$10$...', 'Administrator', 'admin')
ON CONFLICT DO NOTHING;
```

## 3. Scripts de déploiement

### 3.1 Script d'installation
```bash
#!/bin/bash

# Vérification des prérequis
check_prerequisites() {
    command -v docker >/dev/null 2>&1 || { echo "Docker required but not installed. Aborting." >&2; exit 1; }
    command -v docker-compose >/dev/null 2>&1 || { echo "Docker Compose required but not installed. Aborting." >&2; exit 1; }
}

# Création de la structure des dossiers
create_directory_structure() {
    mkdir -p config/{mqtt,nginx,db,nodered}
    mkdir -p data/{db,mqtt,nodered}
    mkdir -p logs/{webapp,api,mqtt,m5stack}
    mkdir -p webapp
    mkdir -p api
    mkdir -p m5stack-manager
}

# Génération des fichiers de configuration
generate_config_files() {
    # Génération des mots de passe
    POSTGRES_PASSWORD=$(openssl rand -base64 32)
    MQTT_PASSWORD=$(openssl rand -base64 32)
    
    # Sauvegarde des secrets
    cat > .env << EOF
POSTGRES_PASSWORD=${POSTGRES_PASSWORD}
MQTT_PASSWORD=${MQTT_PASSWORD}
EOF
}

# Installation
main() {
    check_prerequisites
    create_directory_structure
    generate_config_files
    
    echo "Structure created. Ready to deploy."
}

main