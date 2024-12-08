# Analyse des coûts du projet M5Stack HuskyLens

## Vue d'ensemble

Ce dossier contient l'historique et l'analyse des coûts d'utilisation de Claude AI pour le développement du projet.

### Structure des fichiers

```
coast/
├── claude_api_cost_YYYY_MM.csv    # Fichiers mensuels de coûts
├── README.md                      # Ce fichier
└── scripts/                       # Scripts d'analyse
    └── analyze_claude_costs.py    # Script principal d'analyse
```

### Format des fichiers CSV

Les fichiers de coûts suivent le format suivant :
```csv
usage_date_utc,model,workspace,api_key,usage_type,token_type,cost_usd
2024-12-07,Claude 3.5 Sonnet 2024-10-22,,Stéphane,message,input_no_cache,0.00
```

#### Description des colonnes
- `usage_date_utc` : Date d'utilisation
- `model` : Version de Claude utilisée
- `workspace` : Espace de travail (si applicable)
- `api_key` : Identifiant utilisateur
- `usage_type` : Type d'utilisation (message, etc.)
- `token_type` : Type de tokens (input/output/cache)
- `cost_usd` : Coût en dollars US

### Résumé des coûts

Période d'analyse : Mars 2024 - Décembre 2024

#### Coûts globaux
- Entrée sans cache : $442.36
- Écriture cache : $8.09
- Lecture cache : $11.85
- Sortie : $112.33
- **Total : $574.63**

#### Répartition mensuelle
| Mois      | Coût total |
|-----------|------------|
| 2024/03   | $86.39     |
| 2024/04   | $296.66    |
| 2024/05   | $147.73    |
| 2024/06   | $15.82     |
| 2024/08   | $1.12      |
| 2024/09   | $0.92      |
| 2024/10   | $13.92     |
| 2024/11   | $0.13      |
| 2024/12   | $11.94     |

### Analyse des tendances

1. **Phase initiale (Mars-Mai 2024)**
   - Coûts élevés dus au développement initial
   - Pas d'utilisation du cache
   - Focus sur la génération de code et documentation

2. **Phase d'optimisation (Juin-Septembre 2024)**
   - Réduction significative des coûts
   - Amélioration de l'efficacité des prompts
   - Utilisation plus ciblée de l'IA

3. **Phase actuelle (Octobre-Décembre 2024)**
   - Introduction du système de cache
   - Coûts stabilisés
   - Meilleure efficacité des interactions

### Utilisation des scripts

Pour analyser les coûts :
```bash
cd /chemin/vers/projet
./scripts/analyze_claude_costs.py
```

Le script générera un rapport détaillé dans `claude_cost_analysis.txt`.

### Optimisations futures

1. **Utilisation du cache**
   - Maximiser l'utilisation du cache pour réduire les coûts
   - Optimiser les prompts pour une meilleure réutilisation

2. **Stratégies de réduction des coûts**
   - Prétraitement des requêtes
   - Templates de prompts optimisés
   - Documentation locale pour les requêtes fréquentes