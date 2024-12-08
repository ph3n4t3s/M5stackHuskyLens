#!/usr/bin/env python3
import os
import sys
import shutil
import platform

Import("env")

def get_project_dir():
    """Détermine le chemin du projet en utilisant l'environnement PlatformIO"""
    try:
        # Utilise PROJECTDIR de l'environnement PlatformIO si disponible
        project_dir = env.get('PROJECTDIR', None)
        if project_dir:
            return project_dir
    except:
        pass
    
    try:
        # Essaie d'utiliser __file__ comme fallback
        return os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    except NameError:
        # Dernier recours : utilise le répertoire courant
        current_dir = os.getcwd()
        if os.path.basename(current_dir) == "scripts":
            return os.path.dirname(current_dir)
        return current_dir

PROJECT_DIR = get_project_dir()
DATA_DIR = os.path.join(PROJECT_DIR, "data")

def create_data_dir():
    """Crée le répertoire data s'il n'existe pas"""
    try:
        if not os.path.exists(DATA_DIR):
            os.makedirs(DATA_DIR)
            print(f"Répertoire data créé: {DATA_DIR}")
    except Exception as e:
        print(f"Erreur lors de la création du répertoire data: {e}")
        return False
    return True

def copy_version_file():
    """Copie le fichier VERSION dans le répertoire data"""
    version_file = os.path.join(PROJECT_DIR, "VERSION")
    try:
        if os.path.exists(version_file):
            shutil.copy2(version_file, os.path.join(DATA_DIR, "VERSION"))
            print(f"Fichier VERSION copié vers: {DATA_DIR}")
            return True
        else:
            print(f"Fichier VERSION non trouvé: {version_file}")
            return False
    except Exception as e:
        print(f"Erreur lors de la copie du fichier VERSION: {e}")
        return False

def print_environment_info():
    """Affiche les informations sur l'environnement"""
    print("\nInformations sur l'environnement:")
    print(f"Système d'exploitation: {platform.system()}")
    print(f"Architecture: {platform.machine()}")
    print(f"Python version: {sys.version}")
    print(f"Répertoire de travail actuel: {os.getcwd()}")
    print(f"Répertoire du projet détecté: {PROJECT_DIR}")
    print(f"Répertoire data: {DATA_DIR}")
    print(f"Contenu du répertoire projet:")
    try:
        for item in os.listdir(PROJECT_DIR):
            print(f"  - {item}")
    except Exception as e:
        print(f"  Erreur lors de la lecture du répertoire: {e}")
    print("")

def main():
    """Fonction principale du script de pré-build"""
    print("\n=== Début du script pre-build ===")
    print_environment_info()
    
    success = True
    if not create_data_dir():
        success = False
    if not copy_version_file():
        success = False
        
    print("\n=== Fin du script pre-build ===")
    if not success:
        sys.exit(1)

# Point d'entrée pour PlatformIO
main()