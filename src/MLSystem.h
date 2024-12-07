#pragma once

#include <vector>
#include <map>
#include "Config.h"
#include <EloquentTinyML.h>
#include <eloquent_tinyml/tensorflow.h>

struct MLModel {
    String name;
    std::vector<float> weights;
    int inputSize;
    int outputSize;
    float threshold;
    
    MLModel() : inputSize(0), outputSize(0), threshold(0.5f) {}
};

class MLSystem {
public:
    MLSystem();
    bool begin();
    
    // Gestion des modèles
    void addModel(const String& name, const std::vector<float>& weights,
                 int inputSize, int outputSize, float threshold = 0.5f);
    void removeModel(const String& name);
    bool loadModel(const String& filename);
    bool saveModel(const String& name, const String& filename);
    
    // Entraînement et prédiction
    bool train(const String& modelName, 
              const std::vector<std::vector<float>>& features,
              const std::vector<std::vector<float>>& labels);
    std::vector<float> predict(const String& modelName,
                             const std::vector<float>& input);
    
    // Feature extraction
    static std::vector<float> extractFeatures(const SensorData& data);
    static std::vector<float> normalizeFeatures(const std::vector<float>& features);
    
    // Utilitaires
    std::vector<String> getModelNames() const;
    void setThreshold(const String& modelName, float threshold);
    float getAccuracy(const String& modelName) const;
    
private:
    std::map<String, MLModel> models;
    std::map<String, float> accuracies;
    
    // Tensorflow Lite
    static const int MAX_OPERATIONS = 128;
    static const int ARENA_SIZE = 2 * 1024;
    
    bool validateModel(const MLModel& model) const;
    std::vector<float> preprocessInput(const std::vector<float>& input) const;
    void updateAccuracy(const String& modelName, float accuracy);
    
    // Sérialisation
    bool serializeWeights(const std::vector<float>& weights, const String& filename);
    bool deserializeWeights(std::vector<float>& weights, const String& filename);
};