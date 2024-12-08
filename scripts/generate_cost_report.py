#!/usr/bin/env python3
import os
import sys
import csv
from datetime import datetime, timedelta
import subprocess
import json

def get_git_history():
    """Récupère l'historique Git avec les statistiques"""
    cmd = ['git', 'log', '--numstat', '--pretty=format:{"hash": "%H", "author": "%an", "date": "%ai", "message": "%s"}']
    result = subprocess.run(cmd, capture_output=True, text=True)
    return result.stdout.split('\n')

def parse_git_history(lines):
    """Parse l'historique Git et calcule les métriques"""
    commits = []
    current_commit = None
    
    for line in lines:
        if line.startswith('{'):
            if current_commit:
                commits.append(current_commit)
            current_commit = json.loads(line)
            current_commit['changes'] = []
        elif line.strip() and current_commit:
            added, deleted, file = line.split('\t')
            try:
                current_commit['changes'].append({
                    'added': int(added) if added != '-' else 0,
                    'deleted': int(deleted) if deleted != '-' else 0,
                    'file': file
                })
            except ValueError:
                continue
    
    if current_commit:
        commits.append(current_commit)
    
    return commits

def calculate_cost(changes):
    """Calcule un coût estimé basé sur les modifications"""
    base_cost = 0.0002  # Coût de base par ligne
    
    # Facteurs de coût pour différents types de fichiers
    file_factors = {
        '.cpp': 2.0,    # Code C++ plus complexe
        '.h': 1.5,      # Headers
        '.py': 1.8,     # Scripts Python
        '.md': 1.0,     # Documentation
        '.json': 1.2,   # Fichiers de configuration
        '.yml': 1.2,    # Fichiers YAML
        '.html': 1.3,   # Fichiers web
        '.css': 1.1,    # Styles
        '.js': 1.5      # JavaScript
    }
    
    total_cost = 0
    for change in changes:
        ext = os.path.splitext(change['file'])[1].lower()
        factor = file_factors.get(ext, 1.0)
        
        # Calcul du coût basé sur les lignes ajoutées et supprimées
        lines = change['added'] + change['deleted']
        cost = lines * base_cost * factor
        
        total_cost += cost
    
    return round(total_cost, 2)

def generate_cost_csv():
    """Génère les fichiers CSV de coûts"""
    history = get_git_history()
    commits = parse_git_history(history)
    
    # Organise les commits par mois
    monthly_data = {}
    
    for commit in commits:
        date = datetime.strptime(commit['date'][:10], '%Y-%m-%d')
        month_key = date.strftime('%Y_%m')
        
        if month_key not in monthly_data:
            monthly_data[month_key] = []
            
        cost = calculate_cost(commit['changes'])
        if cost > 0:
            monthly_data[month_key].append({
                'date': commit['date'],
                'hash': commit['hash'],
                'author': commit['author'],
                'message': commit['message'],
                'cost': cost
            })
    
    # Génère un fichier CSV par mois
    for month, data in monthly_data.items():
        filename = f'claude_api_cost_{month}.csv'
        with open(filename, 'w', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(['Date', 'Commit', 'Message', 'Author', 'Cost'])
            for entry in data:
                writer.writerow([
                    entry['date'],
                    entry['hash'][:8],
                    entry['message'],
                    entry['author'],
                    f"{entry['cost']:.2f}"
                ])
        print(f"Généré: {filename}")

if __name__ == '__main__':
    # Vérifie qu'on est dans un dépôt Git
    if not os.path.exists('.git'):
        print("Erreur: Ce script doit être exécuté à la racine d'un dépôt Git")
        sys.exit(1)
    
    generate_cost_csv()