#include "PerformanceManager.h"
#include <esp_heap_caps.h>
#include <esp_system.h>
#include <esp_task_wdt.h>
#include <esp_pm.h>
#include "esp_heap_trace.h"

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
        int core = getOptimalCore(name);
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
    // Compacter le tas
    compactHeap();
    
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
            cores[core].usage += calculateTaskCPUUsage(task.second.handle);
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

template<typename T>
bool PerformanceManager::cacheSet(const String& key, const T& value, uint32_t ttl) {
    if (!m_config.enable_cache) return false;
    
    // Vérifier l'espace disponible
    if (m_cache.size() >= m_config.cache_size) {
        removeExpiredEntries();
        
        // Si toujours plein, supprimer l'entrée la plus ancienne
        if (m_cache.size() >= m_config.cache_size) {
            uint64_t oldest = UINT64_MAX;
            String oldestKey;
            
            for (const auto& entry : m_cache_timestamps) {
                if (entry.second < oldest) {
                    oldest = entry.second;
                    oldestKey = entry.first;
                }
            }
            
            if (!oldestKey.isEmpty()) {
                cacheInvalidate(oldestKey);
            }
        }
    }
    
    // Stocker la valeur
    m_cache[key] = value;
    m_cache_timestamps[key] = getCurrentTimestamp();
    m_cache_ttls[key] = ttl > 0 ? ttl : m_config.default_cache_ttl;
    
    return true;
}

template<typename T>
bool PerformanceManager::cacheGet(const String& key, T& value) {
    if (!m_config.enable_cache) return false;
    
    auto it = m_cache.find(key);
    if (it == m_cache.end()) return false;
    
    // Vérifier si l'entrée est expirée
    uint64_t now = getCurrentTimestamp();
    if (now - m_cache_timestamps[key] > m_cache_ttls[key]) {
        cacheInvalidate(key);
        return false;
    }
    
    try {
        value = std::any_cast<T>(it->second);
        return true;
    } catch (const std::bad_any_cast&) {
        return false;
    }
}

void PerformanceManager::cleanupCache() {
    if (!m_config.enable_cache) return;
    
    removeExpiredEntries();
    
    // Vérifier l'utilisation mémoire
    if (calculateMemoryUsage() > m_config.memory_threshold) {
        // Réduire la taille du cache de moitié
        size_t toRemove = m_cache.size() / 2;
        for (size_t i = 0; i < toRemove && !m_cache.empty(); i++) {
            auto it = m_cache.begin();
            cacheInvalidate(it->first);
        }
    }
}

float PerformanceManager::calculateCPUUsage() {
    static uint64_t lastTotalTime = 0;
    static uint64_t lastIdleTime = 0;
    
    uint64_t totalTime = esp_timer_get_time();
    uint64_t idleTime = xTaskGetTickCount() * portTICK_PERIOD_MS * 1000; // en microsecondes
    
    float usage = 0.0f;
    if (lastTotalTime > 0) {
        uint64_t totalDelta = totalTime - lastTotalTime;
        uint64_t idleDelta = idleTime - lastIdleTime;
        usage = 100.0f * (1.0f - static_cast<float>(idleDelta) / totalDelta);
    }
    
    lastTotalTime = totalTime;
    lastIdleTime = idleTime;
    
    return usage;
}

float PerformanceManager::calculateMemoryUsage() {
    uint32_t total = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
    uint32_t free = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    return 100.0f * (1.0f - static_cast<float>(free) / total);
}

float PerformanceManager::measureTemperature() {
    // Note: Cette fonction est une simulation car l'ESP32 n'a pas de capteur de température intégré
    // Dans une implémentation réelle, il faudrait utiliser un capteur externe
    return 40.0f + (rand() % 20); // Simulation entre 40°C et 60°C
}

void PerformanceManager::identifyMemoryLeaks() {
    // Configuration du traçage mémoire
    heap_trace_init_standalone(heap_trace_record, 100);
    
    // Démarrer le traçage
    heap_trace_start(HEAP_TRACE_LEAKS);
    
    // Attendre un peu pour collecter des données
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Arrêter le traçage
    heap_trace_stop();
    
    // Analyser les résultats
    size_t count = heap_trace_get_count();
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            heap_trace_record_t record;
            heap_trace_get(i, &record);
            
            // Loguer les allocations non libérées
            log_e("Memory leak detected: %d bytes at %p", 
                  record.size, record.address);
        }
    }
}

void PerformanceManager::compactHeap() {
    heap_caps_defrag(MALLOC_CAP_DEFAULT);
    heap_caps_defrag(MALLOC_CAP_SPIRAM);
}