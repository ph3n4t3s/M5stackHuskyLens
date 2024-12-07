#include "MetaLearningSystem.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>

MetaLearningSystem::MetaLearningSystem() {}

bool MetaLearningSystem::begin() {
    // TensorFlow support désactivé
    // Initialiser le stockage
    return SPIFFS.begin(true);
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
    
    // Méta-apprentissage factice pour la démo
    for (int step = 0; step < m_config.num_outer_steps; step++) {
        // Mise à jour aléatoire des poids
        for (float& w : model.weights) {
            w += ((esp_random() / (float)UINT32_MAX) * 2.0f - 1.0f) * 0.1f;
        }
        
        // Enregistrer la courbe d'apprentissage
        if (step % 10 == 0) {
            float loss = static_cast<float>(step) / m_config.num_outer_steps;
            m_learning_curves.back().push_back(loss);
        }
    }
    
    // Évaluer les performances finales
    model.performance = evaluate(model_name, tasks[0]);
}

float MetaLearningSystem::evaluate(const String& model_name,
                                const LearningTask& task) {
    auto it = m_models.find(model_name);
    if (it == m_models.end()) return 0.0f;
    
    // Retourner une performance factice pour la démo
    return 0.75f + ((esp_random() / (float)UINT32_MAX) * 0.1f);
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