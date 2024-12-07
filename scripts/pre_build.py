Import("env")
import os
import shutil
import sys

def apply_patches():
    """Applique les patches nécessaires aux bibliothèques"""
    print("Application des patches...")
    
    # Chemin vers les patches
    patches_dir = os.path.join(env.get('PROJECT_DIR'), "patches")
    if not os.path.exists(patches_dir):
        print("Dossier patches non trouvé")
        return
        
    # Chemin vers les bibliothèques
    lib_dir = os.path.join(env.get('PROJECT_DIR'), ".pio", "libdeps", env.get('PIOENV'))
    if not os.path.exists(lib_dir):
        print("Dossier lib non trouvé")
        return
        
    # Application du patch pour HuskyLens
    huskylens_dir = os.path.join(lib_dir, "HuskyLens")
    if os.path.exists(huskylens_dir):
        patch_file = os.path.join(patches_dir, "mac_compatibility.patch")
        if os.path.exists(patch_file):
            os.system(f"patch -d {huskylens_dir} -p1 < {patch_file}")
            print("Patch HuskyLens appliqué")

def copy_libs():
    """Copie les bibliothèques nécessaires"""
    print("Configuration des bibliothèques...")
    
    # Exemple de copie de bibliothèque si nécessaire
    # src = os.path.join(env.get('PROJECT_DIR'), "lib", "custom_lib")
    # dst = os.path.join(env.get('PROJECT_DIR'), ".pio", "libdeps", env.get('PIOENV'), "custom_lib")
    # if os.path.exists(src) and not os.path.exists(dst):
    #     shutil.copytree(src, dst)

def pre_build():
    """Fonction principale de pré-build"""
    print("Exécution des scripts de pré-build...")
    
    # Application des patches
    apply_patches()
    
    # Copie des bibliothèques
    copy_libs()
    
    print("Configuration terminée")

# Exécution du script
pre_build()