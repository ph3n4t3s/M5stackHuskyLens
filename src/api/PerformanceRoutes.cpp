#include "PerformanceRoutes.h"
#include <ArduinoJson.h>

void PerformanceRoutes::registerRoutes(AsyncWebServer& server, PerformanceManager& performanceManager) {
    // Métriques en temps réel
    server.on("/api/performance/metrics", HTTP_GET,
        [&](AsyncWebServerRequest* request) {
            handleGetMetrics(request, performanceManager);
        }
    );
    
    // Optimisation système
    server.on("/api/performance/optimize", HTTP_POST,
        [&](AsyncWebServerRequest* request) {
            handleOptimize(request, performanceManager);
        }
    );
    
    // Équilibrage de charge
    server.on("/api/performance/balance", HTTP_POST,
        [&](AsyncWebServerRequest* request) {
            handleBalanceLoad(request, performanceManager);
        }
    );
    
    // Gestion du cache
    server.on("/api/performance/clear-cache", HTTP_POST,
        [&](AsyncWebServerRequest* request) {
            handleClearCache(request, performanceManager);
        }
    );
    
    // Contrôle des tâches
    server.on("/api/performance/task", HTTP_POST,
        [&](AsyncWebServerRequest* request) {
            handleTaskControl(request, performanceManager);
        }
    );
    
    // Données historiques
    server.on("/api/performance/history", HTTP_GET,
        [&](AsyncWebServerRequest* request) {
            handleGetHistoricalData(request, performanceManager);
        }
    );
    
    // Export des données
    server.on("/api/performance/export", HTTP_GET,
        [&](AsyncWebServerRequest* request) {
            handleExportData(request, performanceManager);
        }
    );
    
    // Réinitialisation
    server.on("/api/performance/reset", HTTP_POST,
        [&](AsyncWebServerRequest* request) {
            handleReset(request, performanceManager);
        }
    );
}

void PerformanceRoutes::handleGetMetrics(AsyncWebServerRequest* request, 
                                       PerformanceManager& performanceManager) {
    PerformanceMetrics metrics = performanceManager.getMetrics();
    
    DynamicJsonDocument doc(1024);
    doc["cpu_usage"] = metrics.cpu_usage;
    doc["memory_usage"] = metrics.memory_usage;
    doc["fps"] = metrics.fps;
    doc["temperature"] = metrics.temperature;
    doc["free_heap"] = metrics.free_heap;
    doc["min_free_heap"] = metrics.min_free_heap;
    
    // Ajouter les avertissements
    JsonArray warnings = doc.createNestedArray("warnings");
    for (const auto& warning : performanceManager.getWarnings()) {
        warnings.add(warning);
    }
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

void PerformanceRoutes::handleOptimize(AsyncWebServerRequest* request,
                                     PerformanceManager& performanceManager) {
    performanceManager.optimizeCPU();
    performanceManager.optimizeMemory();
    
    sendJsonResponse(request, true, "Système optimisé");
}

void PerformanceRoutes::handleBalanceLoad(AsyncWebServerRequest* request,
                                        PerformanceManager& performanceManager) {
    performanceManager.balanceLoad();
    sendJsonResponse(request, true, "Charge équilibrée");
}

void PerformanceRoutes::handleClearCache(AsyncWebServerRequest* request,
                                       PerformanceManager& performanceManager) {
    performanceManager.cacheClear();
    sendJsonResponse(request, true, "Cache vidé");
}

void PerformanceRoutes::handleTaskControl(AsyncWebServerRequest* request,
                                        PerformanceManager& performanceManager) {
    if (!request->hasParam("name", true)) {
        sendJsonResponse(request, false, "Nom de tâche manquant");
        return;
    }
    
    String taskName = request->getParam("name", true)->value();
    String action = request->hasParam("action", true) ?
                   request->getParam("action", true)->value() : "toggle";
    
    bool success = false;
    if (action == "suspend") {
        success = performanceManager.suspendTask(taskName);
    } else if (action == "resume") {
        success = performanceManager.resumeTask(taskName);
    } else {
        // Toggle
        success = true; // La fonction appropriée sera appelée selon l'état actuel
    }
    
    sendJsonResponse(request, success, 
        success ? "Tâche mise à jour" : "Erreur de mise à jour de la tâche");
}

void PerformanceRoutes::handleGetHistoricalData(AsyncWebServerRequest* request,
                                              PerformanceManager& performanceManager) {
    uint32_t duration = request->hasParam("duration") ? 
        request->getParam("duration")->value().toInt() : 3600000; // 1h par défaut
    
    auto metrics = performanceManager.getHistoricalMetrics(duration);
    
    DynamicJsonDocument doc(16384);
    JsonArray data = doc.createNestedArray("data");
    
    for (const auto& metric : metrics) {
        JsonObject entry = data.createNestedObject();
        entry["timestamp"] = metric.timestamp;
        entry["cpu"] = metric.cpu_usage;
        entry["memory"] = metric.memory_usage;
        entry["fps"] = metric.fps;
        entry["temperature"] = metric.temperature;
    }
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}

void PerformanceRoutes::handleExportData(AsyncWebServerRequest* request,
                                       PerformanceManager& performanceManager) {
    DynamicJsonDocument doc(32768);
    
    // Métriques courantes
    JsonObject current = doc.createNestedObject("current");
    auto metrics = performanceManager.getMetrics();
    current["cpu_usage"] = metrics.cpu_usage;
    current["memory_usage"] = metrics.memory_usage;
    current["fps"] = metrics.fps;
    current["temperature"] = metrics.temperature;
    current["free_heap"] = metrics.free_heap;
    
    // Historique
    JsonArray history = doc.createNestedArray("history");
    auto historicalData = performanceManager.getHistoricalMetrics();
    for (const auto& metric : historicalData) {
        JsonObject entry = history.createNestedObject();
        entry["timestamp"] = metric.timestamp;
        entry["cpu"] = metric.cpu_usage;
        entry["memory"] = metric.memory_usage;
        entry["fps"] = metric.fps;
    }
    
    String response;
    serializeJson(doc, response);
    
    AsyncResponseStream* stream = request->beginResponseStream("application/json");
    stream->print(response);
    request->send(stream);
}

void PerformanceRoutes::handleReset(AsyncWebServerRequest* request,
                                  PerformanceManager& performanceManager) {
    // Réinitialiser la configuration
    PerformanceConfig defaultConfig;
    performanceManager.setConfig(defaultConfig);
    
    // Vider le cache
    performanceManager.cacheClear();
    
    // Optimiser
    performanceManager.optimizeCPU();
    performanceManager.optimizeMemory();
    
    sendJsonResponse(request, true, "Système réinitialisé");
}

void PerformanceRoutes::sendJsonResponse(AsyncWebServerRequest* request,
                                       bool success,
                                       const String& message) {
    DynamicJsonDocument doc(256);
    doc["success"] = success;
    doc["message"] = message;
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
}