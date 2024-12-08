#!/usr/bin/env python3
import os
import csv
from datetime import datetime
import glob
from collections import defaultdict

def analyze_cost_files(directory):
    """Analyse les fichiers de coûts Claude et génère un rapport"""
    # Structure pour stocker les données agrégées
    monthly_data = defaultdict(lambda: {
        'input_no_cache': 0.0,
        'input_cache_write': 0.0,
        'input_cache_read': 0.0,
        'output': 0.0
    })
    
    # Parcours tous les fichiers CSV
    csv_files = glob.glob(os.path.join(directory, 'claude_api_cost_*.csv'))
    for file_path in csv_files:
        with open(file_path, 'r') as f:
            reader = csv.DictReader(f)
            for row in reader:
                month = row['usage_date_utc'][:7].replace('-', '_')
                cost = float(row['cost_usd'])
                usage_type = row['token_type']
                monthly_data[month][usage_type] += cost

    return monthly_data

def generate_detailed_report(data):
    """Génère un rapport détaillé des coûts"""
    total_input_no_cache = sum(month_data['input_no_cache'] for month_data in data.values())
    total_input_cache_write = sum(month_data['input_cache_write'] for month_data in data.values())
    total_input_cache_read = sum(month_data['input_cache_read'] for month_data in data.values())
    total_output = sum(month_data['output'] for month_data in data.values())
    total_cost = sum(sum(costs.values()) for costs in data.values())
    
    report = f"""
Rapport détaillé des coûts d'utilisation de Claude AI

Période d'analyse: {min(data.keys()).replace('_', '/')} à {max(data.keys()).replace('_', '/')}

Statistiques globales:
---------------------
Coût entrée sans cache:    ${total_input_no_cache:,.2f}
Coût écriture cache:       ${total_input_cache_write:,.2f}
Coût lecture cache:        ${total_input_cache_read:,.2f}
Coût sortie:              ${total_output:,.2f}
Coût total:               ${total_cost:,.2f}

Détail par mois:
--------------"""
    
    for month in sorted(data.keys()):
        month_data = data[month]
        month_total = sum(month_data.values())
        
        report += f"""
{month.replace('_', '/')}:
  Entrée sans cache:     ${month_data['input_no_cache']:,.2f}
  Écriture cache:        ${month_data['input_cache_write']:,.2f}
  Lecture cache:         ${month_data['input_cache_read']:,.2f}
  Sortie:               ${month_data['output']:,.2f}
  Total:                ${month_total:,.2f}"""
    
    with open('claude_cost_analysis.txt', 'w') as f:
        f.write(report)
    
    print("Rapport détaillé généré dans claude_cost_analysis.txt")
    return total_cost

def main():
    # Chemin vers le dossier contenant les fichiers CSV
    csv_dir = 'docs/coast'
    
    # Analyse des coûts
    monthly_data = analyze_cost_files(csv_dir)
    
    # Génération du rapport
    total_cost = generate_detailed_report(monthly_data)
    
    print(f"\nCoût total du projet: ${total_cost:,.2f}")

if __name__ == '__main__':
    main()