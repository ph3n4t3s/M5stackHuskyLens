#pragma once

#include <vector>
#include <map>
#include "../Config.h"
#include <tensorflow/lite/micro/micro_interpreter.h>

// Structure pour un méta-modèle
struct MetaModel {
    String name;
    std::vector<float> weights;
    std::vector<int> architecture;
    float learning_rate;
    float meta_learning_rate;
    uint32_t training_steps;
    float performance;
    
    MetaModel() : 
        learning_rate(0.01f),
        meta_learning_rate(0.001f),
        training_steps(0),
        performance(0.0f) {}
};

// Structure pour une tâche d'apprentissage
struct LearningTask {
    String name;
    std::vector<std::vector<float>> support_set;
    std::vector<std::vector<float>> query_set;
    std::vector<int> labels;
    int num_classes;
    int input_dim;
    bool is_regression;
    
    LearningTask() : 
        num_classes(0),
        input_dim(0),
        is_regression(false) {}
};

// Configuration du méta-apprentissage
struct MetaLearningConfig {
    int num_inner_steps;
    int num_outer_steps;
    int meta_batch_size;
    float inner_learning_rate;
    float outer_learning_rate;
    bool use_first_order;
    bool use_meta_sgd;
    String optimizer;
    
    MetaLearningConfig() :
        num_inner_steps(5),
        num_outer_steps(100),
        meta_batch_size(4),
        inner_learning_rate(0.01f),
        outer_learning_rate(0.001f),
        use_first_order(true),
        use_meta_sgd(false),
        optimizer("adam") {}
};

class MetaLearningSystem {
public:
    MetaLearningSystem();
    bool begin();
    
    // Gestion des modèles
    void addMetaModel(const String& name, const std::vector<int>& architecture);
    void removeMetaModel(const String& name);
    bool saveMetaModel(const String& name, const String& path);
    bool loadMetaModel(const String& name, const String& path);
    
    // Gestion des tâches
    void addTask(const LearningTask& task);
    void removeTask(const String& name);
    std::vector<String> listTasks() const;
    
    // Meta-apprentissage
    void trainMetaModel(const String& model_name,
                       const std::vector<String>& task_names);
    std::vector<float> adapt(const String& model_name,
                           const LearningTask& task,
                           int num_steps);
    float evaluate(const String& model_name,
                  const LearningTask& task);
    
    // Configuration
    void setConfig(const MetaLearningConfig& config);
    MetaLearningConfig getConfig() const;
    
    // Optimisation
    void optimizeHyperparameters(const String& model_name,
                                const std::vector<String>& task_names);
    void searchArchitecture(const String& model_name,
                          const std::vector<String>& task_names);
    
    // Analyse et diagnostics
    float getMeanPerformance(const String& model_name) const;
    std::vector<float> getLearningCurve(const String& model_name) const;
    void analyzeTasks(const std::vector<String>& task_names);
    
    // Export et visualisation
    bool exportMetrics(const String& filename);
    String generateReport(const String& model_name);
    void visualizeLearning();

private:
    MetaLearningConfig m_config;
    std::map<String, MetaModel> m_models;
    std::map<String, LearningTask> m_tasks;
    std::vector<std::vector<float>> m_learning_curves;
    
    // TensorFlow Lite
    std::unique_ptr<tflite::MicroInterpreter> m_interpreter;
    std::unique_ptr<tflite::ErrorReporter> m_error_reporter;
    const tflite::Model* m_model;
    TfLiteTensor* m_input_tensor;
    TfLiteTensor* m_output_tensor;
    
    // Méta-apprentissage interne
    void innerLoop(MetaModel& model, const LearningTask& task);
    void outerLoop(MetaModel& model, const std::vector<LearningTask>& tasks);
    std::vector<float> computeMetaGradient(const MetaModel& model,
                                         const std::vector<LearningTask>& tasks);
    
    // Optimisation
    float computeMetaLoss(const MetaModel& model,
                         const std::vector<LearningTask>& tasks);
    void updateMetaParameters(MetaModel& model,
                            const std::vector<float>& gradients);
    
    // Gestion de la mémoire
    void allocateBuffers(const MetaModel& model);
    void deallocateBuffers();
    
    // Utilitaires
    std::vector<float> forward(const MetaModel& model,
                             const std::vector<float>& input);
    std::vector<float> backward(const MetaModel& model,
                              const std::vector<float>& gradients);
    void updateModelWeights(MetaModel& model,
                          const std::vector<float>& gradients,
                          float learning_rate);
};