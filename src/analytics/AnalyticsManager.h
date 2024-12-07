#pragma once

#include <vector>
#include <deque>
#include <map>
#include "../Config.h"
#include <TensorFlowLite_ESP32.h>

// Structure pour les métriques d'analyse
struct AnalyticsMetrics {
    // Métriques de détection
    float detection_accuracy;
    float false_positive_rate;
    float false_negative_rate;
    float average_confidence;
    
    // Métriques de comportement
    float movement_patterns[8];
    float interaction_frequency;
    float dwell_time;
    float transition_probability[8][8];
    
    // Métriques temporelles
    uint32_t processing_time;
    uint32_t detection_latency;
    uint32_t response_time;
    
    // Métriques de qualité
    float image_quality;
    float lighting_condition;
    float motion_blur;
    float noise_level;
    
    AnalyticsMetrics() {
        reset();
    }
    
    void reset() {
        detection_accuracy = 0.0f;
        false_positive_rate = 0.0f;
        false_negative_rate = 0.0f;
        average_confidence = 0.0f;
        interaction_frequency = 0.0f;
        dwell_time = 0.0f;
        processing_time = 0;
        detection_latency = 0;
        response_time = 0;
        image_quality = 0.0f;
        lighting_condition = 0.0f;
        motion_blur = 0.0f;
        noise_level = 0.0f;
        
        for (int i = 0; i < 8; i++) {
            movement_patterns[i] = 0.0f;
            for (int j = 0; j < 8; j++) {
                transition_probability[i][j] = 0.0f;
            }
        }
    }
};

// Structure pour l'apprentissage fédéré
struct FederatedModel {
    String model_id;
    std::vector<float> weights;
    float version;
    uint64_t timestamp;
    std::vector<String> contributors;
    float aggregation_weight;
    
    FederatedModel() : version(1.0f), timestamp(0), aggregation_weight(1.0f) {}
};

// Configuration des analytics
struct AnalyticsConfig {
    bool enable_federated_learning;
    bool enable_behavior_analysis;
    bool enable_quality_metrics;
    bool enable_advanced_metrics;
    uint32_t metrics_interval;
    uint32_t model_update_interval;
    float learning_rate;
    float contribution_threshold;
    String federation_server;
    
    AnalyticsConfig() :
        enable_federated_learning(true),
        enable_behavior_analysis(true),
        enable_quality_metrics(true),
        enable_advanced_metrics(true),
        metrics_interval(1000),
        model_update_interval(3600000),
        learning_rate(0.01f),
        contribution_threshold(0.8f),
        federation_server("https://federation.example.com") {}
};

class AnalyticsManager {
public:
    AnalyticsManager();
    bool begin();
    
    // Configuration
    void setConfig(const AnalyticsConfig& config);
    AnalyticsConfig getConfig() const;
    
    // Métriques et analyse
    void updateMetrics(const SensorData& data);
    AnalyticsMetrics getMetrics() const;
    std::vector<AnalyticsMetrics> getHistoricalMetrics(uint32_t duration) const;
    
    // Analyse comportementale
    void analyzeBehavior(const SensorData& data);
    std::vector<float> getMovementPatterns() const;
    std::vector<std::vector<float>> getTransitionMatrix() const;
    
    // Apprentissage fédéré
    bool updateModel(const FederatedModel& model);
    FederatedModel getLocalModel() const;
    bool contributeToFederation();
    void synchronizeWithFederation();
    
    // Qualité et performance
    float evaluateImageQuality(const SensorData& data);
    float measureDetectionAccuracy();
    void optimizeParameters();
    
    // Prédiction et recommandations
    std::vector<float> predictBehavior(const SensorData& data);
    std::vector<String> getRecommendations();
    float calculateConfidenceScore();
    
    // Export et visualisation
    bool exportAnalytics(const String& filename);
    String generateReport();
    void visualizePatterns();

private:
    AnalyticsConfig m_config;
    AnalyticsMetrics m_current_metrics;
    std::deque<AnalyticsMetrics> m_historical_metrics;
    FederatedModel m_local_model;
    std::map<String, float> m_behavior_patterns;
    
    // Monitoring interne
    TaskHandle_t m_analytics_task;
    bool m_analytics_running;
    SemaphoreHandle_t m_metrics_mutex;
    
    // TensorFlow Lite
    std::unique_ptr<tflite::MicroInterpreter> m_interpreter;
    std::unique_ptr<tflite::ErrorReporter> m_error_reporter;
    const tflite::Model* m_model;
    TfLiteTensor* m_input_tensor;
    TfLiteTensor* m_output_tensor;
    
    // Méthodes internes
    void processMetrics();
    void updateBehaviorModel();
    void aggregateContributions();
    float calculateModelQuality();
    void optimizeFederatedLearning();
    
    // Analyse comportementale
    void detectPatterns();
    void updateTransitionMatrix();
    void calculateProbabilities();
    
    // Apprentissage fédéré
    bool validateModel(const FederatedModel& model);
    void mergeModels(const FederatedModel& remote_model);
    float calculateContributionScore();
    
    // Qualité et métriques
    float analyzeImageNoise(const SensorData& data);
    float detectMotionBlur();
    float assessLightingConditions();
    
    // Prédiction
    std::vector<float> runInference(const std::vector<float>& input);
    void updatePredictionModel();
    float validatePredictions();
    
    // Utilitaires
    void logAnalytics(const String& event);
    void checkThresholds();
    void cleanupOldData();
};