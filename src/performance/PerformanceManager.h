#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <vector>
#include <queue>
#include <map>
#include <any>
#include "Config.h"

// Structure pour les statistiques de performance
struct PerformanceMetrics {
    float cpu_usage;
    float memory_usage;
    float fps;
    float processing_time;
    float network_latency;
    uint32_t free_heap;
    uint32_t min_free_heap;
    float temperature;
    uint64_t timestamp;
    
    PerformanceMetrics() :
        cpu_usage(0),
        memory_usage(0),
        fps(0),
        processing_time(0),
        network_latency(0),
        free_heap(0),
        min_free_heap(0),
        temperature(0),
        timestamp(0) {}
};

// Structure pour une tâche planifiée
struct ScheduledTask {
    String name;
    TaskFunction_t function;
    void* parameters;
    uint32_t stackSize;
    UBaseType_t priority;
    TaskHandle_t handle;
    bool running;
    
    ScheduledTask() :
        function(nullptr),
        parameters(nullptr),
        stackSize(4096),
        priority(1),
        handle(nullptr),
        running(false) {}
};

// Structure pour le cache
template<typename T>
struct CacheEntry {
    T data;
    uint64_t timestamp;
    uint32_t ttl;
    bool valid;
};

// Configuration du gestionnaire de performances
struct PerformanceConfig {
    bool enable_multicore;
    bool enable_cache;
    bool enable_monitoring;
    uint32_t cache_size;
    uint32_t default_cache_ttl;
    uint32_t monitoring_interval;
    float cpu_threshold;
    float memory_threshold;
    float temperature_threshold;
    
    PerformanceConfig() :
        enable_multicore(true),
        enable_cache(true),
        enable_monitoring(true),
        cache_size(1024 * 1024),  // 1MB
        default_cache_ttl(60000), // 1 minute
        monitoring_interval(1000), // 1 seconde
        cpu_threshold(80.0f),     // 80%
        memory_threshold(80.0f),  // 80%
        temperature_threshold(70.0f) // 70°C
    {}
};

class PerformanceManager {
public:
    PerformanceManager();
    bool begin();
    
    // Gestion des tâches
    bool scheduleTask(const String& name, TaskFunction_t function, void* parameters = nullptr,
                     uint32_t stackSize = 4096, UBaseType_t priority = 1);
    bool unscheduleTask(const String& name);
    bool suspendTask(const String& name);
    bool resumeTask(const String& name);
    
    // Cache
    template<typename T>
    bool cacheSet(const String& key, const T& value, uint32_t ttl = 0);
    
    template<typename T>
    bool cacheGet(const String& key, T& value);
    
    void cacheInvalidate(const String& key);
    void cacheClear();
    
    // Monitoring
    PerformanceMetrics getMetrics();
    void setThresholds(float cpu, float memory, float temperature);
    bool isSystemHealthy();
    std::vector<String> getWarnings();
    
    // Configuration
    void setConfig(const PerformanceConfig& config);
    PerformanceConfig getConfig() const;
    
    // Optimisation
    void optimizeMemory();
    void optimizeCPU();
    void balanceLoad();
    
    // Statistiques
    void logMetrics();
    std::vector<PerformanceMetrics> getHistoricalMetrics(uint32_t duration = 3600000);

private:
    PerformanceConfig m_config;
    std::map<String, ScheduledTask> m_tasks;
    std::map<String, std::any> m_cache;
    std::map<String, uint64_t> m_cache_timestamps;
    std::map<String, uint32_t> m_cache_ttls;
    std::queue<PerformanceMetrics> m_metrics_history;
    std::vector<String> m_warnings;
    
    // Monitoring interne
    TaskHandle_t m_monitoring_task;
    bool m_monitoring_running;
    SemaphoreHandle_t m_metrics_mutex;
    
    // Méthodes internes
    static void monitoringTask(void* parameter);
    int getOptimalCore(const String& taskName) const;
    float calculateTaskCPUUsage(TaskHandle_t handle) const;
    void updateMetrics();
    void checkThresholds();
    void cleanupCache();
    uint32_t calculateOptimalStackSize(const String& taskName);
    UBaseType_t calculateOptimalPriority(const String& taskName);
    void distributeTasksAcrossCores();
    
    // Cache helpers
    bool isKeyValid(const String& key);
    void removeExpiredEntries();
    uint64_t getCurrentTimestamp();
    
    // Métriques helpers
    float calculateCPUUsage();
    float calculateMemoryUsage();
    float measureTemperature();
    float calculateProcessingTime(const String& taskName);
    float measureNetworkLatency();
    
    // Optimisation helpers
    void compactHeap();
    void defragmentMemory();
    void adjustTaskPriorities();
    void monitorStackUsage();
    void identifyMemoryLeaks();
};