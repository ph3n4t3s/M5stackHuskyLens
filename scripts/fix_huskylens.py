import os
import shutil
import platform

Import("env")

# Chemin vers le fichier HUSKYLENS.h
HUSKYLENS_PATH = os.path.join(env.get("PROJECT_LIBDEPS_DIR"), 
                             env.get("PIOENV"), 
                             "HuskyLens", 
                             "HUSKYLENS.h")

def patch_huskylens():
    """Applique les corrections pour macOS M2"""
    if not os.path.exists(HUSKYLENS_PATH):
        print("HUSKYLENS.h non trouvé à", HUSKYLENS_PATH)
        return

    # Sauvegarde du fichier original
    backup_path = HUSKYLENS_PATH + ".bak"
    if not os.path.exists(backup_path):
        shutil.copy2(HUSKYLENS_PATH, backup_path)

    with open(HUSKYLENS_PATH, "r") as f:
        content = f.read()

    # Correction des fonctions problématiques
    content = content.replace(
        'protocolPtr = (Protocol_t*) realloc(protocolPtr, max(protocolInfo.protocolSize, 1) * sizeof(Protocol_t));',
        'protocolPtr = (Protocol_t*) realloc(protocolPtr, ((protocolInfo.protocolSize > 1) ? protocolInfo.protocolSize : 1) * sizeof(Protocol_t));'
    )
    content = content.replace(
        'currentIndex = min(currentIndex, result);',
        'currentIndex = (currentIndex < result) ? currentIndex : result;'
    )

    with open(HUSKYLENS_PATH, "w") as f:
        f.write(content)

    print(f"HuskyLens patch appliqué pour {platform.system()} {platform.machine()}")

# Point d'entrée pour PlatformIO
patch_huskylens()