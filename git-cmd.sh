#!/bin/bash

# Vérifie que le bon nombre d'arguments est passé au script
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <action>"
    echo "Actions possibles : clone, commit-push"
    exit 1
fi

# Variables fixes
USERNAME="ph3nat3s"
TOKEN="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"
REPO_URL="https://github.com/ph3n4t3s/M5stackHuskyLens.git"  # Remplacez par l'URL de votre dépôt
REPO_PATH="/home/computeruse/M5stackHuskyLens"  # Chemin local où le dépôt sera cloné ou mis à jour
COMMIT_MESSAGE="Votre message de commit"  # Message de commit par défaut
BRANCH_NAME="main"  # Nom de la branche par défaut

# Action à effectuer (clone ou commit-push)
ACTION=$1

if [ "$ACTION" == "clone" ]; then
    # Clone le dépôt en utilisant le token d'accès personnel
    git clone https://$USERNAME:$TOKEN@${REPO_URL#https://} $REPO_PATH
elif [ "$ACTION" == "commit-push" ]; then
    # Se déplace vers le répertoire du dépôt
    cd $REPO_PATH || exit
    
    # Met à jour la branche locale avec les dernières modifications du dépôt distant
    git pull origin $BRANCH_NAME

    # Ajoute toutes les modifications au suivi de Git
    git add -A

    # Crée un commit avec le message fourni
    git commit -m "$COMMIT_MESSAGE"

    # Pousse les modifications vers la branche spécifiée du dépôt distant
    git push https://$USERNAME:$TOKEN@${REPO_URL#https://} $BRANCH_NAME
else
    echo "Action non reconnue : $ACTION"
    echo "Actions possibles : clone, commit-push"
    exit 1
fi
