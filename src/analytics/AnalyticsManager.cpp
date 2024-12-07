#include "AnalyticsManager.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <HTTPClient.h>

AnalyticsManager::AnalyticsManager() : 
    m_analytics_running(false),
    m_analytics_task(nullptr) {
    m_metrics_mutex = xSemaphoreCreateMutex();
}

int AnalyticsManager::classifyPosition(const Point& point) {
    // Divise l'écran en 8 zones et classifie la position
    const int GRID_SIZE = 3;
    int col = (point.x * GRID_SIZE) / 320;
    int row = (point.y * GRID_SIZE) / 240;
    return row * GRID_SIZE + col;
}

void AnalyticsManager::normalizeTransitionMatrix() {
    // Normaliser chaque ligne de la matrice de transition
    for (int i = 0; i < 8; i++) {
        float sum = 0;
        for (int j = 0; j < 8; j++) {
            sum += m_current_metrics.transition_probability[i][j];
        }
        if (sum > 0) {
            for (int j = 0; j < 8; j++) {
                m_current_metrics.transition_probability[i][j] /= sum;
            }
        }
    }
}

std::vector<float> AnalyticsManager::extractFeatures(const SensorData& data) {
    std::vector<float> features;
    
    if (data.points.empty()) {
        return features;
    }
    
    // Calculer le centre de masse
    float avg_x = 0, avg_y = 0;
    for (const auto& p : data.points) {
        avg_x += p.x;
        avg_y += p.y;
    }
    avg_x /= data.points.size();
    avg_y /= data.points.size();
    
    // Ajouter les caractéristiques
    features.push_back(avg_x / 320.0f);  // Normaliser par la largeur
    features.push_back(avg_y / 240.0f);  // Normaliser par la hauteur
    features.push_back(data.confidence);
    features.push_back(data.points.size() / 10.0f); // Normaliser par un max arbitraire
    
    return features;
}

// Le reste du fichier reste inchangé...