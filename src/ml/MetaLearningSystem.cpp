#include "MetaLearningSystem.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>

MetaLearningSystem::MetaLearningSystem() {}

bool MetaLearningSystem::begin() {
    // Initialiser TensorFlow Lite
    m_error_reporter = std::make_unique<tflite::MicroErrorReporter>();
    
    // Initialiser le stockage
    if (!SPIFFS.begin(true)) {
        return false;
    }
    
    return true;
}

void MetaLearningSystem::addMetaModel(const String& name,
                                    const std::vector<int>& architecture) {
    MetaModel model;
    model.name = name;
    model.architecture = architecture;
    
    // Initialiser les poids aléatoirement
    int num_weights = 0;
    for (size_t i = 1; i < architecture.size(); i++) {
        num_weights += architecture[i-1] * architecture[i]; // Poids
        num_weights += architecture[i];                     // Biais
    }
    
    model.weights.resize(num_weights);
    for (float& w : model.weights) {
        w = (esp_random() / (float)UINT32_MAX) * 2.0f - 1.0f;
    }
    
    m_models[name] = model;
}

void MetaLearningSystem::trainMetaModel(const String& model_name,
                                      const std::vector<String>& task_names) {
    auto it = m_models.find(model_name);
    if (it == m_models.end()) return;
    
    MetaModel& model = it->second;
    std::vector<LearningTask> tasks;
    
    // Collecter les tâches
    for (const auto& name : task_names) {
        auto task_it = m_tasks.find(name);
        if (task_it != m_tasks.end()) {
            tasks.push_back(task_it->second);
        }
    }
    
    // Méta-apprentissage
    for (int step = 0; step < m_config.num_outer_steps; step++) {
        // Sélectionner un mini-batch de tâches
        std::vector<LearningTask> batch;
        for (int i = 0; i < m_config.meta_batch_size; i++) {
            int idx = esp_random() % tasks.size();
            batch.push_back(tasks[idx]);
        }
        
        // Boucle externe
        outerLoop(model, batch);
        
        // Enregistrer la courbe d'apprentissage
        if (step % 10 == 0) {
            float loss = computeMetaLoss(model, batch);
            m_learning_curves.back().push_back(loss);
        }
    }
    
    // Évaluer les performances finales
    model.performance = evaluate(model_name, tasks[0]);
}

void MetaLearningSystem::outerLoop(MetaModel& model,
                                 const std::vector<LearningTask>& tasks) {
    // Sauvegarder les poids originaux
    std::vector<float> original_weights = model.weights;
    
    // Calculer le méta-gradient
    std::vector<float> meta_gradient = computeMetaGradient(model, tasks);
    
    // Mettre à jour les méta-paramètres
    updateMetaParameters(model, meta_gradient);
    
    // Restaurer les poids originaux
    model.weights = original_weights;
}

std::vector<float> MetaLearningSystem::computeMetaGradient(
    const MetaModel& model,
    const std::vector<LearningTask>& tasks) {
    
    std::vector<float> meta_gradient(model.weights.size(), 0.0f);
    
    for (const auto& task : tasks) {
        // Copier le modèle pour l'adaptation
        MetaModel adapted_model = model;
        
        // Adaptation à la tâche
        for (int step = 0; step < m_config.num_inner_steps; step++) {
            innerLoop(adapted_model, task);
        }
        
        // Calculer le gradient sur l'ensemble de requête
        std::vector<float> task_gradient = computeTaskGradient(adapted_model, task);
        
        // Accumuler le méta-gradient
        for (size_t i = 0; i < meta_gradient.size(); i++) {
            meta_gradient[i] += task_gradient[i] / tasks.size();
        }
    }
    
    return meta_gradient;
}

void MetaLearningSystem::innerLoop(MetaModel& model, const LearningTask& task) {
    // Sélectionner un mini-batch du support set
    std::vector<std::vector<float>> batch_inputs;
    std::vector<int> batch_labels;
    
    const int batch_size = 8;
    for (int i = 0; i < batch_size; i++) {
        int idx = esp_random() % task.support_set.size();
        batch_inputs.push_back(task.support_set[idx]);
        batch_labels.push_back(task.labels[idx]);
    }
    
    // Forward pass
    std::vector<float> predictions;
    for (const auto& input : batch_inputs) {
        auto output = forward(model, input);
        predictions.insert(predictions.end(), output.begin(), output.end());
    }
    
    // Calculer le gradient
    std::vector<float> gradients = computeLossGradient(predictions, batch_labels);
    
    // Mise à jour des poids
    updateModelWeights(model, gradients, m_config.inner_learning_rate);
}

float MetaLearningSystem::evaluate(const String& model_name,
                                 const LearningTask& task) {
    auto it = m_models.find(model_name);
    if (it == m_models.end()) return 0.0f;
    
    const MetaModel& model = it->second;
    float total_loss = 0.0f;
    
    // Évaluer sur l'ensemble de requête
    for (size_t i = 0; i < task.query_set.size(); i++) {
        auto predictions = forward(model, task.query_set[i]);
        
        // Calculer la perte
        if (task.is_regression) {
            // Erreur quadratique moyenne
            float mse = 0.0f;
            for (size_t j = 0; j < predictions.size(); j++) {
                float diff = predictions[j] - task.labels[i];
                mse += diff * diff;
            }
            total_loss += mse / predictions.size();
        } else {
            // Cross-entropy pour la classification
            int true_label = task.labels[i];
            float cross_entropy = -log(predictions[true_label] + 1e-7f);
            total_loss += cross_entropy;
        }
    }
    
    return total_loss / task.query_set.size();
}

std::vector<float> MetaLearningSystem::forward(const MetaModel& model,
                                             const std::vector<float>& input) {
    if (!m_interpreter || !m_input_tensor || !m_output_tensor) {
        return std::vector<float>();
    }
    
    // Copier l'entrée
    memcpy(m_input_tensor->data.f, input.data(),
           input.size() * sizeof(float));
    
    // Inférence
    TfLiteStatus status = m_interpreter->Invoke();
    if (status != kTfLiteOk) {
        return std::vector<float>();
    }
    
    // Copier la sortie
    std::vector<float> output(m_output_tensor->bytes / sizeof(float));
    memcpy(output.data(), m_output_tensor->data.f,
           m_output_tensor->bytes);
    
    return output;
}

void MetaLearningSystem::updateModelWeights(MetaModel& model,
                                          const std::vector<float>& gradients,
                                          float learning_rate) {
    for (size_t i = 0; i < model.weights.size(); i++) {
        model.weights[i] -= learning_rate * gradients[i];
    }
}

bool MetaLearningSystem::saveMetaModel(const String& name, const String& path) {
    auto it = m_models.find(name);
    if (it == m_models.end()) return false;
    
    const MetaModel& model = it->second;
    
    // Créer le document JSON
    DynamicJsonDocument doc(16384);
    doc["name"] = model.name;
    doc["performance"] = model.performance;
    doc["training_steps"] = model.training_steps;
    doc["learning_rate"] = model.learning_rate;
    doc["meta_learning_rate"] = model.meta_learning_rate;
    
    JsonArray arch = doc.createNestedArray("architecture");
    for (int layer : model.architecture) {
        arch.add(layer);
    }
    
    JsonArray weights = doc.createNestedArray("weights");
    for (float w : model.weights) {
        weights.add(w);
    }
    
    // Sauvegarder le fichier
    File file = SPIFFS.open(path, "w");
    if (!file) return false;
    
    serializeJson(doc, file);
    file.close();
    
    return true;
}

bool MetaLearningSystem::loadMetaModel(const String& name, const String& path) {
    File file = SPIFFS.open(path, "r");
    if (!file) return false;
    
    DynamicJsonDocument doc(16384);
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) return false;
    
    // Créer le modèle
    MetaModel model;
    model.name = doc["name"] | name;
    model.performance = doc["performance"] | 0.0f;
    model.training_steps = doc["training_steps"] | 0;
    model.learning_rate = doc["learning_rate"] | 0.01f;
    model.meta_learning_rate = doc["meta_learning_rate"] | 0.001f;
    
    JsonArray arch = doc["architecture"];
    for (JsonVariant v : arch) {
        model.architecture.push_back(v.as<int>());
    }
    
    JsonArray weights = doc["weights"];
    for (JsonVariant v : weights) {
        model.weights.push_back(v.as<float>());
    }
    
    m_models[name] = model;
    return true;
}

void MetaLearningSystem::optimizeHyperparameters(const String& model_name,
                                               const std::vector<String>& task_names) {
    // TODO: Implémenter l'optimisation des hyperparamètres
    // - Recherche par grille
    // - Optimisation bayésienne
    // - Méta-gradients sur les hyperparamètres
}

void MetaLearningSystem::searchArchitecture(const String& model_name,
                                          const std::vector<String>& task_names) {
    // TODO: Implémenter la recherche d'architecture
    // - Neural Architecture Search (NAS)
    // - Évolution différentielle
    // - Méta-apprentissage d'architecture
}