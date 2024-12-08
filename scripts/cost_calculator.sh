#!/usr/bin/env bash
set -euo pipefail

# Forcer la locale C pour éviter les problèmes de décimales
export LC_ALL=C

# On récupère le nom d'utilisateur s'il est fourni en argument
user_filter="${1:-}"

# Création d'un fichier temporaire pour stocker les données nettoyées
tmpfile=$(mktemp)

# Nettoyer les éventuels retours chariot Windows et concaténer les fichiers
# Si vous avez plusieurs fichiers, on les traite tous.
cat claude_api_cost_*.csv | tr -d '\r' > "$tmpfile"

# Condition de filtre : 
# Si $user_filter est vide, on ne filtre pas sur le champ 4.
# Si $user_filter n'est pas vide, on ajoute la condition `$4==user_filter`.

if [ -n "$user_filter" ]; then
    # Filtrer seulement l'utilisateur spécifié
    awk -F"," -v user="$user_filter" '
        NR>1 && $4==user { sum += ($5+0) } 
        END { printf("Coût total pour %s: $%.2f\n", user, sum) }
    ' "$tmpfile"
else
    # Pas d'utilisateur spécifié, on somme tous les coûts
    awk -F"," '
        NR>1 { sum += ($5+0) } 
        END { printf("Coût total du projet: $%.2f\n", sum) }
    ' "$tmpfile"
fi

rm -f "$tmpfile"