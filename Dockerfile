# Utiliser l'image de base Python pour PlatformIO
FROM python:3.11-slim

# Installation des dépendances système
RUN apt-get update && apt-get install -y \
    git \
    curl \
    build-essential \
    udev \
    && rm -rf /var/lib/apt/lists/*

# Installation de PlatformIO
RUN python -m pip install --no-cache-dir platformio

# Configuration de l'environnement
ENV PLATFORMIO_CORE_DIR=/platformio

# Dossier de travail
WORKDIR /workspace

# Script pour lancer la compilation
COPY scripts/build.sh /usr/local/bin/build.sh
RUN chmod +x /usr/local/bin/build.sh

ENTRYPOINT ["/usr/local/bin/build.sh"]