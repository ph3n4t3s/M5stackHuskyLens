#!/usr/bin/env python3

import os
import json
import subprocess
import sys

# Variables globales
PROJECT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ENV_NAME = "esp32-s3-dev"

class BuildEnvironment:
    def __init__(self):
        self.project_dir = PROJECT_DIR
        self.env_name = ENV_NAME

    def get(self, key):
        if key == 'PROJECT_DIR':
            return self.project_dir
        elif key == 'PIOENV':
            return self.env_name
        return None

def load_config():
    """Charge la configuration depuis config.json"""
    config_path = os.path.join(PROJECT_DIR, "scripts", "config.json")
    try:
        with open(config_path, 'r') as f:
            return json.load(f)
    except Exception as e:
        print(f"Erreur lors du chargement de la configuration : {e}")
        return {}

def apply_patches(env):
    """Applique les patches nécessaires aux bibliothèques"""
    print("Application des patches...")
    
    # Chemin vers les patches
    patches_dir = os.path.join(env.get('PROJECT_DIR'), "patches")
    if not os.path.exists(patches_dir):
        print("Dossier patches non trouvé")
        return False
        
    # Chemin vers les bibliothèques
    lib_dir = os.path.join(env.get('PROJECT_DIR'), ".pio", "libdeps", env.get('PIOENV'))
    if not os.path.exists(lib_dir):
        print("Dossier lib non trouvé")
        return False
        
    # Chargement de la configuration
    config = load_config()
    if not config or 'patches' not in config:
        print("Configuration des patches non trouvée")
        return False

    success = True
    for lib_name, patch_info in config['patches'].items():
        lib_dir_path = os.path.join(lib_dir, patch_info['target_dir'])
        patch_file = os.path.join(patches_dir, patch_info['patch_file'])
        
        if os.path.exists(lib_dir_path) and os.path.exists(patch_file):
            try:
                result = subprocess.run(
                    ['patch', '-d', lib_dir_path, '-p1', '-i', patch_file],
                    capture_output=True,
                    text=True
                )
                if result.returncode == 0:
                    print(f"Patch appliqué avec succès pour {lib_name}")
                else:
                    print(f"Erreur lors de l'application du patch pour {lib_name}")
                    print(f"Sortie : {result.stderr}")
                    success = False
            except Exception as e:
                print(f"Erreur lors de l'application du patch pour {lib_name}: {e}")
                success = False
    
    return success

def check_dependencies(env):
    """Vérifie et installe les dépendances manquantes"""
    print("Vérification des dépendances...")
    
    config = load_config()
    if not config or 'libraries' not in config:
        print("Configuration des bibliothèques non trouvée")
        return False
        
    lib_dir = os.path.join(env.get('PROJECT_DIR'), ".pio", "libdeps", env.get('PIOENV'))
    if not os.path.exists(lib_dir):
        os.makedirs(lib_dir)
        
    success = True
    for lib_name, lib_info in config['libraries'].items():
        lib_path = os.path.join(lib_dir, lib_name)
        if not os.path.exists(lib_path):
            try:
                # Clone du dépôt Git
                result = subprocess.run(
                    ['git', 'clone', '--branch', lib_info['version'], lib_info['repository'], lib_path],
                    capture_output=True,
                    text=True
                )
                if result.returncode == 0:
                    print(f"Bibliothèque {lib_name} installée avec succès")
                else:
                    print(f"Erreur lors de l'installation de {lib_name}")
                    print(f"Sortie : {result.stderr}")
                    success = False
            except Exception as e:
                print(f"Erreur lors de l'installation de {lib_name}: {e}")
                success = False
                
    return success

def pre_build():
    """Fonction principale de pré-build"""
    print("Exécution des scripts de pré-build...")
    
    # Création de l'environnement
    if 'env' in globals():
        build_env = env
    else:
        build_env = BuildEnvironment()
    
    # Vérification des dépendances
    if not check_dependencies(build_env):
        print("Erreur lors de la vérification des dépendances")
        return False
    
    # Application des patches
    if not apply_patches(build_env):
        print("Erreur lors de l'application des patches")
        return False
    
    print("Configuration terminée avec succès")
    return True

if __name__ == "__main__":
    success = pre_build()
    sys.exit(0 if success else 1)