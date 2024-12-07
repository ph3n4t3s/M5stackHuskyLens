#pragma once

#include <ESPAsyncWebSrv.h>
#include "../performance/PerformanceManager.h"

class PerformanceRoutes {
public:
    static void registerRoutes(AsyncWebServer& server, PerformanceManager& performanceManager);
    
private:
    static void handleGetMetrics(AsyncWebServerRequest* request, PerformanceManager& performanceManager);
    static void handleOptimize(AsyncWebServerRequest* request, PerformanceManager& performanceManager);
    static void handleBalanceLoad(AsyncWebServerRequest* request, PerformanceManager& performanceManager);
    static void handleClearCache(AsyncWebServerRequest* request, PerformanceManager& performanceManager);
    static void handleTaskControl(AsyncWebServerRequest* request, PerformanceManager& performanceManager);
    static void handleGetHistoricalData(AsyncWebServerRequest* request, PerformanceManager& performanceManager);
    static void handleExportData(AsyncWebServerRequest* request, PerformanceManager& performanceManager);
    static void handleReset(AsyncWebServerRequest* request, PerformanceManager& performanceManager);
    
    static void sendJsonResponse(AsyncWebServerRequest* request, bool success, const String& message);
};