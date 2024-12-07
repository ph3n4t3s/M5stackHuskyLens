#include "AnalyticsManager.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <HTTPClient.h>
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"

AnalyticsManager::AnalyticsManager() : 
    m_analytics_running(false),
    m_analytics_task(nullptr) {
    m_metrics_mutex = xSemaphoreCreateMutex();
}

bool AnalyticsManager::begin() {
    // Initialiser TensorFlow Lite
    static tflite::AllOpsResolver resolver;
    m_error_reporter = std::make_unique<tflite::MicroErrorReporter>();
    
    // Charger le modèle
    if (!loadTFLiteModel()) {
        return false;
    }
    
    // Démarrer la tâche d'analytics
    if (m_config.enable_advanced_metrics) {
        xTaskCreatePinnedToCore(
            [](void* parameter) {
                AnalyticsManager* manager = static_cast<AnalyticsManager*>(parameter);
                manager->processMetrics();
                vTaskDelete(nullptr);
            },
            "analytics",
            8192,
            this,
            1,
            &m_analytics_task,
            0
        );
    }
    
    return true;
}

void AnalyticsManager::updateMetrics(const SensorData& data) {
    if (xSemaphoreTake(m_metrics_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        // Mesurer le temps de traitement
        uint32_t start_time = millis();
        
        // Analyser la qualité de l'image
        m_current_metrics.image_quality = evaluateImageQuality(data);
        m_current_metrics.motion_blur = detectMotionBlur();
        m_current_metrics.lighting_condition = assessLightingConditions();
        
        // Analyser les performances de détection
        if (!data.points.empty()) {
            m_current_metrics.detection_latency = millis() - data.timestamp;
            m_current_metrics.average_confidence = data.confidence;
        }
        
        // Analyse comportementale si activée
        if (m_config.enable_behavior_analysis) {
            analyzeBehavior(data);
        }
        
        // Mesurer le temps total de traitement
        m_current_metrics.processing_time = millis() - start_time;
        
        // Ajouter aux métriques historiques
        m_historical_metrics.push_back(m_current_metrics);
        while (m_historical_metrics.size() > 3600) {
            m_historical_metrics.pop_front();
        }
        
        xSemaphoreGive(m_metrics_mutex);
    }
}

void AnalyticsManager::analyzeBehavior(const SensorData& data) {
    static std::vector<Point> last_positions;
    static uint32_t last_update = 0;
    
    if (data.points.empty()) {
        return;
    }
    
    // Calculer les mouvements
    if (!last_positions.empty()) {
        std::vector<float> movements;
        for (size_t i = 0; i < data.points.size(); i++) {
            if (i < last_positions.size()) {
                float dx = data.points[i].x - last_positions[i].x;
                float dy = data.points[i].y - last_positions[i].y;
                float magnitude = sqrt(dx*dx + dy*dy);
                float direction = atan2(dy, dx);
                
                // Classifier le mouvement en 8 directions
                int pattern_index = ((int)round(direction * 4/M_PI) + 8) % 8;
                m_current_metrics.movement_patterns[pattern_index] += magnitude;
            }
        }
    }
    
    // Mettre à jour la matrice de transition
    if (!last_positions.empty()) {
        for (size_t i = 0; i < data.points.size() && i < last_positions.size(); i++) {
            int from_state = classifyPosition(last_positions[i]);
            int to_state = classifyPosition(data.points[i]);
            m_current_metrics.transition_probability[from_state][to_state] += 1.0f;
        }
        normalizeTransitionMatrix();
    }
    
    // Calculer le temps d'interaction
    if (last_update > 0) {
        m_current_metrics.dwell_time += (millis() - last_update) / 1000.0f;
    }
    
    last_positions = data.points;
    last_update = millis();
}

float AnalyticsManager::evaluateImageQuality(const SensorData& data) {
    float quality = 1.0f;
    
    // Analyser le bruit
    float noise = analyzeImageNoise(data);
    quality *= (1.0f - noise);
    
    // Analyser le flou
    float blur = detectMotionBlur();
    quality *= (1.0f - blur);
    
    // Analyser l'éclairage
    float lighting = assessLightingConditions();
    quality *= lighting;
    
    return quality;
}

bool AnalyticsManager::contributeToFederation() {
    if (!m_config.enable_federated_learning) {
        return false;
    }
    
    // Vérifier la qualité du modèle local
    float quality = calculateModelQuality();
    if (quality < m_config.contribution_threshold) {
        return false;
    }
    
    // Préparer la contribution
    DynamicJsonDocument doc(16384);
    doc["model_id"] = m_local_model.model_id;
    doc["version"] = m_local_model.version;
    doc["quality"] = quality;
    
    JsonArray weights = doc.createNestedArray("weights");
    for (float w : m_local_model.weights) {
        weights.add(w);
    }
    
    // Envoyer au serveur de fédération
    HTTPClient http;
    http.begin(m_config.federation_server + "/contribute");
    http.addHeader("Content-Type", "application/json");
    
    String payload;
    serializeJson(doc, payload);
    
    int httpCode = http.POST(payload);
    bool success = (httpCode == HTTP_CODE_OK);
    
    http.end();
    return success;
}

void AnalyticsManager::synchronizeWithFederation() {
    if (!m_config.enable_federated_learning) {
        return;
    }
    
    HTTPClient http;
    http.begin(m_config.federation_server + "/model");
    
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        DynamicJsonDocument doc(16384);
        deserializeJson(doc, payload);
        
        FederatedModel remote_model;
        remote_model.model_id = doc["model_id"].as<String>();
        remote_model.version = doc["version"];
        
        JsonArray weights = doc["weights"];
        for (JsonVariant w : weights) {
            remote_model.weights.push_back(w.as<float>());
        }
        
        // Fusionner avec le modèle local
        mergeModels(remote_model);
    }
    
    http.end();
}

void AnalyticsManager::mergeModels(const FederatedModel& remote_model) {
    if (m_local_model.weights.size() != remote_model.weights.size()) {
        return;
    }
    
    float alpha = calculateContributionScore();
    float beta = 1.0f - alpha;
    
    for (size_t i = 0; i < m_local_model.weights.size(); i++) {
        m_local_model.weights[i] = alpha * m_local_model.weights[i] +
                                  beta * remote_model.weights[i];
    }
    
    m_local_model.version = std::max(m_local_model.version, remote_model.version);
    m_local_model.timestamp = millis();
}

std::vector<float> AnalyticsManager::predictBehavior(const SensorData& data) {
    std::vector<float> features = extractFeatures(data);
    return runInference(features);
}

std::vector<float> AnalyticsManager::runInference(const std::vector<float>& input) {
    if (!m_interpreter || !m_input_tensor || !m_output_tensor) {
        return std::vector<float>();
    }
    
    // Copier les données d'entrée
    for (size_t i = 0; i < input.size() && i < m_input_tensor->bytes / sizeof(float); i++) {
        m_input_tensor->data.f[i] = input[i];
    }
    
    // Exécuter l'inférence
    TfLiteStatus status = m_interpreter->Invoke();
    if (status != kTfLiteOk) {
        return std::vector<float>();
    }
    
    // Récupérer les résultats
    std::vector<float> output(m_output_tensor->bytes / sizeof(float));
    memcpy(output.data(), m_output_tensor->data.f, m_output_tensor->bytes);
    
    return output;
}

bool AnalyticsManager::exportAnalytics(const String& filename) {
    File file = SPIFFS.open(filename, "w");
    if (!file) {
        return false;
    }
    
    DynamicJsonDocument doc(32768);
    
    // Métriques actuelles
    JsonObject current = doc.createNestedObject("current");
    serializeMetrics(current, m_current_metrics);
    
    // Historique
    JsonArray history = doc.createNestedArray("history");
    for (const auto& metrics : m_historical_metrics) {
        JsonObject entry = history.createNestedObject();
        serializeMetrics(entry, metrics);
    }
    
    // Modèle local
    JsonObject model = doc.createNestedObject("model");
    model["id"] = m_local_model.model_id;
    model["version"] = m_local_model.version;
    model["quality"] = calculateModelQuality();
    
    serializeJson(doc, file);
    file.close();
    
    return true;
}

void AnalyticsManager::serializeMetrics(JsonObject& obj, const AnalyticsMetrics& metrics) {
    obj["detection_accuracy"] = metrics.detection_accuracy;
    obj["false_positive_rate"] = metrics.false_positive_rate;
    obj["false_negative_rate"] = metrics.false_negative_rate;
    obj["average_confidence"] = metrics.average_confidence;
    obj["processing_time"] = metrics.processing_time;
    obj["image_quality"] = metrics.image_quality;
    
    JsonArray patterns = obj.createNestedArray("movement_patterns");
    for (int i = 0; i < 8; i++) {
        patterns.add(metrics.movement_patterns[i]);
    }
    
    JsonArray transitions = obj.createNestedArray("transitions");
    for (int i = 0; i < 8; i++) {
        JsonArray row = transitions.createNestedArray();
        for (int j = 0; j < 8; j++) {
            row.add(metrics.transition_probability[i][j]);
        }
    }
}