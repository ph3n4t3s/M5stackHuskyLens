#include "MLSystem.h"
#include <SPIFFS.h>
#include <ArduinoJson.h>

MLSystem::MLSystem() {}

bool MLSystem::begin() {
    return SPIFFS.begin(true);
}

void MLSystem::addModel(const String& name, const std::vector<float>& weights,
                       int inputSize, int outputSize, float threshold) {
    MLModel model;
    model.name = name;
    model.weights = weights;
    model.inputSize = inputSize;
    model.outputSize = outputSize;
    model.threshold = threshold;
    
    if (validateModel(model)) {
        models[name] = model;
    }
}

void MLSystem::removeModel(const String& name) {
    models.erase(name);
    accuracies.erase(name);
}

bool MLSystem::loadModel(const String& filename) {
    File file = SPIFFS.open(filename, "r");
    if (!file) return false;
    
    DynamicJsonDocument doc(16384);
    DeserializationError error = deserializeJson(doc, file);
    if (error) {
        file.close();
        return false;
    }
    
    String name = doc["name"] | "unnamed";
    int inputSize = doc["inputSize"] | 0;
    int outputSize = doc["outputSize"] | 0;
    float threshold = doc["threshold"] | 0.5f;
    
    std::vector<float> weights;
    JsonArray weightsArray = doc["weights"];
    for (JsonVariant v : weightsArray) {
        weights.push_back(v.as<float>());
    }
    
    file.close();
    
    addModel(name, weights, inputSize, outputSize, threshold);
    return true;
}

bool MLSystem::saveModel(const String& name, const String& filename) {
    auto it = models.find(name);
    if (it == models.end()) return false;
    
    DynamicJsonDocument doc(16384);
    doc["name"] = it->second.name;
    doc["inputSize"] = it->second.inputSize;
    doc["outputSize"] = it->second.outputSize;
    doc["threshold"] = it->second.threshold;
    
    JsonArray weightsArray = doc.createNestedArray("weights");
    for (float w : it->second.weights) {
        weightsArray.add(w);
    }
    
    File file = SPIFFS.open(filename, "w");
    if (!file) return false;
    
    serializeJson(doc, file);
    file.close();
    return true;
}

bool MLSystem::train(const String& modelName,
                    const std::vector<std::vector<float>>& features,
                    const std::vector<std::vector<float>>& labels) {
    auto it = models.find(modelName);
    if (it == models.end()) return false;
    
    // Validation des données
    if (features.empty() || features.size() != labels.size()) return false;
    if (features[0].size() != it->second.inputSize) return false;
    if (labels[0].size() != it->second.outputSize) return false;
    
    // TODO: Implémenter l'entraînement avec TensorFlow Lite
    // Pour l'instant, nous utilisons un modèle pré-entraîné
    
    return true;
}

std::vector<float> MLSystem::predict(const String& modelName,
                                   const std::vector<float>& input) {
    auto it = models.find(modelName);
    if (it == models.end()) return std::vector<float>();
    
    if (input.size() != it->second.inputSize) return std::vector<float>();
    
    std::vector<float> preprocessed = preprocessInput(input);
    
    // TODO: Implémenter la prédiction avec TensorFlow Lite
    // Pour l'instant, retourne des valeurs simulées
    std::vector<float> output(it->second.outputSize, 0.0f);
    
    return output;
}

std::vector<float> MLSystem::extractFeatures(const SensorData& data) {
    std::vector<float> features;
    
    // Position moyenne
    float avgX = 0, avgY = 0;
    for (const auto& p : data.points) {
        avgX += p.x;
        avgY += p.y;
    }
    if (!data.points.empty()) {
        avgX /= data.points.size();
        avgY /= data.points.size();
    }
    features.push_back(avgX);
    features.push_back(avgY);
    
    // Nombre d'objets
    features.push_back(data.objectCount);
    
    // Confiance
    features.push_back(data.confidence);
    
    // Dispersion des points
    float dispersionX = 0, dispersionY = 0;
    for (const auto& p : data.points) {
        dispersionX += (p.x - avgX) * (p.x - avgX);
        dispersionY += (p.y - avgY) * (p.y - avgY);
    }
    if (!data.points.empty()) {
        dispersionX /= data.points.size();
        dispersionY /= data.points.size();
    }
    features.push_back(sqrt(dispersionX));
    features.push_back(sqrt(dispersionY));
    
    return features;
}

std::vector<float> MLSystem::normalizeFeatures(const std::vector<float>& features) {
    if (features.empty()) return features;
    
    std::vector<float> normalized = features;
    float minVal = *std::min_element(features.begin(), features.end());
    float maxVal = *std::max_element(features.begin(), features.end());
    
    if (maxVal > minVal) {
        for (auto& f : normalized) {
            f = (f - minVal) / (maxVal - minVal);
        }
    }
    
    return normalized;
}

std::vector<String> MLSystem::getModelNames() const {
    std::vector<String> names;
    for (const auto& model : models) {
        names.push_back(model.first);
    }
    return names;
}

void MLSystem::setThreshold(const String& modelName, float threshold) {
    auto it = models.find(modelName);
    if (it != models.end()) {
        it->second.threshold = threshold;
    }
}

float MLSystem::getAccuracy(const String& modelName) const {
    auto it = accuracies.find(modelName);
    return (it != accuracies.end()) ? it->second : 0.0f;
}

bool MLSystem::validateModel(const MLModel& model) const {
    if (model.inputSize <= 0 || model.outputSize <= 0) return false;
    if (model.weights.empty()) return false;
    if (model.threshold < 0.0f || model.threshold > 1.0f) return false;
    
    // TODO: Ajouter plus de validations
    
    return true;
}

std::vector<float> MLSystem::preprocessInput(const std::vector<float>& input) const {
    // Normalisation
    std::vector<float> preprocessed = normalizeFeatures(input);
    
    // TODO: Ajouter d'autres étapes de prétraitement si nécessaire
    
    return preprocessed;
}

void MLSystem::updateAccuracy(const String& modelName, float accuracy) {
    accuracies[modelName] = accuracy;
}

bool MLSystem::serializeWeights(const std::vector<float>& weights, const String& filename) {
    File file = SPIFFS.open(filename, "w");
    if (!file) return false;
    
    file.write((uint8_t*)weights.data(), weights.size() * sizeof(float));
    file.close();
    return true;
}

bool MLSystem::deserializeWeights(std::vector<float>& weights, const String& filename) {
    File file = SPIFFS.open(filename, "r");
    if (!file) return false;
    
    size_t size = file.size() / sizeof(float);
    weights.resize(size);
    file.read((uint8_t*)weights.data(), size * sizeof(float));
    file.close();
    return true;
}