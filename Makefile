.PHONY: build clean docker-build

# Variables
DOCKER_COMPOSE = docker-compose
BUILD_DIR = .pio/build
OUTPUT_DIR = output

# Commandes principales
build:
	pio run

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(OUTPUT_DIR)

docker-build:
	$(DOCKER_COMPOSE) build
	$(DOCKER_COMPOSE) run --rm builder

docker-clean:
	$(DOCKER_COMPOSE) down -v
	make clean

# Commandes pour le développement
monitor:
	pio device monitor

upload:
	pio run -t upload

all: clean build