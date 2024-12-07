#include "PerformanceManager.h"
#include <esp_heap_caps.h>
#include <esp_system.h>
#include <esp_task_wdt.h>
#include <esp_pm.h>

PerformanceManager::PerformanceManager() : 
    m_monitoring_running(false),
    m_monitoring_task(nullptr) {
    m_metrics_mutex = xSemaphoreCreateMutex();
}

bool PerformanceManager::begin() {
    // Initialiser le monitoring
    if (m_config.enable_monitoring) {
        xTaskCreatePinnedToCore(
            monitoringTask,
            "monitor",
            4096,
            this,
            1,
            &m_monitoring_task,
            0  // Core 0
        );
    }
    
    // Initialiser le multicore si activé
    if (m_config.enable_multicore) {
        distributeTasksAcrossCores();
    }
    
    return true;
}

bool PerformanceManager::scheduleTask(const String& name, TaskFunction_t function,
                                    void* parameters, uint32_t stackSize, UBaseType_t priority) {
    if (m_tasks.find(name) != m_tasks.end()) {
        return false; // La tâche existe déjà
    }
    
    ScheduledTask task;
    task.name = name;
    task.function = function;
    task.parameters = parameters;
    task.stackSize = calculateOptimalStackSize(name);
    task.priority = calculateOptimalPriority(name);
    
    // Créer la tâche
    BaseType_t result;
    if (m_config.enable_multicore) {
        // Déterminer le meilleur core pour cette tâche
        int core = 0; // Fallback si getOptimalCore n'est pas implémenté
        result = xTaskCreatePinnedToCore(
            function,
            name.c_str(),
            task.stackSize,
            task.parameters,
            task.priority,
            &task.handle,
            core
        );
    } else {
        result = xTaskCreate(
            function,
            name.c_str(),
            task.stackSize,
            task.parameters,
            task.priority,
            &task.handle
        );
    }
    
    if (result != pdPASS) {
        return false;
    }
    
    task.running = true;
    m_tasks[name] = task;
    
    return true;
}

int PerformanceManager::getOptimalCore(const String& taskName) const {
    // Logique simplifiée pour déterminer le meilleur core
    static int nextCore = 0;
    return nextCore++ % 2;  // Alterner entre core 0 et 1
}

float PerformanceManager::calculateTaskCPUUsage(TaskHandle_t handle) const {
    if (!handle) return 0.0f;
    
    // Version simplifiée - retourne une valeur simulée
    return (float)(rand() % 100);
}

void PerformanceManager::monitoringTask(void* parameter) {
    PerformanceManager* manager = static_cast<PerformanceManager*>(parameter);
    TickType_t lastWakeTime = xTaskGetTickCount();
    
    while (manager->m_monitoring_running) {
        manager->updateMetrics();
        manager->checkThresholds();
        manager->cleanupCache();
        
        // Attendre l'intervalle configuré
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(manager->m_config.monitoring_interval));
    }
    
    vTaskDelete(nullptr);
}

void PerformanceManager::updateMetrics() {
    if (xSemaphoreTake(m_metrics_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        PerformanceMetrics metrics;
        
        metrics.cpu_usage = calculateCPUUsage();
        metrics.memory_usage = calculateMemoryUsage();
        metrics.temperature = measureTemperature();
        metrics.free_heap = esp_get_free_heap_size();
        metrics.min_free_heap = esp_get_minimum_free_heap_size();
        
        // Calculer le FPS moyen
        static uint32_t lastFrameTime = 0;
        uint32_t currentTime = millis();
        if (lastFrameTime != 0) {
            metrics.fps = 1000.0f / (currentTime - lastFrameTime);
        }
        lastFrameTime = currentTime;
        
        // Ajouter aux métriques historiques
        m_metrics_history.push(metrics);
        while (m_metrics_history.size() > 3600) { // Garder 1h d'historique
            m_metrics_history.pop();
        }
        
        xSemaphoreGive(m_metrics_mutex);
    }
}

void PerformanceManager::checkThresholds() {
    m_warnings.clear();
    
    auto metrics = getMetrics();
    
    if (metrics.cpu_usage > m_config.cpu_threshold) {
        m_warnings.push_back("CPU usage high: " + String(metrics.cpu_usage) + "%");
        optimizeCPU();
    }
    
    if (metrics.memory_usage > m_config.memory_threshold) {
        m_warnings.push_back("Memory usage high: " + String(metrics.memory_usage) + "%");
        optimizeMemory();
    }
    
    if (metrics.temperature > m_config.temperature_threshold) {
        m_warnings.push_back("Temperature high: " + String(metrics.temperature) + "°C");
    }
}

void PerformanceManager::optimizeMemory() {
    // Nettoyer le cache si nécessaire
    if (calculateMemoryUsage() > m_config.memory_threshold) {
        cacheClear();
    }
    
    // Identifier et gérer les fuites mémoire
    identifyMemoryLeaks();
}

void PerformanceManager::optimizeCPU() {
    // Ajuster les priorités des tâches
    adjustTaskPriorities();
    
    // Rééquilibrer la charge entre les cores
    if (m_config.enable_multicore) {
        balanceLoad();
    }
    
    // Suspendre les tâches non critiques si nécessaire
    for (auto& task : m_tasks) {
        if (task.second.priority < 3) { // Tâches basse priorité
            suspendTask(task.first);
        }
    }
}

void PerformanceManager::balanceLoad() {
    struct CoreLoad {
        float usage;
        std::vector<String> tasks;
    };
    
    std::array<CoreLoad, 2> cores;
    
    // Calculer la charge par core
    for (const auto& task : m_tasks) {
        if (!task.second.running) continue;
        
        int core = xTaskGetAffinity(task.second.handle);
        if (core >= 0 && core < 2) {
            cores[core].tasks.push_back(task.first);
            cores[core].usage += 50.0f;  // Valeur par défaut de 50% par tâche
        }
    }
    
    // Rééquilibrer si nécessaire
    if (std::abs(cores[0].usage - cores[1].usage) > 20.0f) { // Différence > 20%
        // Déplacer des tâches du core le plus chargé vers le moins chargé
        int sourceCore = cores[0].usage > cores[1].usage ? 0 : 1;
        int targetCore = 1 - sourceCore;
        
        for (const auto& taskName : cores[sourceCore].tasks) {
            auto& task = m_tasks[taskName];
            
            // Recréer la tâche sur l'autre core
            TaskHandle_t newHandle;
            if (xTaskCreatePinnedToCore(
                task.function,
                task.name.c_str(),
                task.stackSize,
                task.parameters,
                task.priority,
                &newHandle,
                targetCore
            ) == pdPASS) {
                // Supprimer l'ancienne tâche
                vTaskDelete(task.handle);
                task.handle = newHandle;
            }
        }
    }
}

void PerformanceManager::identifyMemoryLeaks() {
    // Version simplifiée sans heap tracing
    cleanupCache();
    monitorStackUsage();
}

// Le reste des méthodes reste inchangé...