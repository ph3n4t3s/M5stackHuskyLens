# API Web Server Implementation

## Overview

This directory contains the web server implementation for the M5Stack HuskyLens project. It provides a wrapper around ESPAsyncWebServer to ensure consistent usage across the project.

## Key Files

- `WebServer.h`: Main header file with adaptations for different ESPAsyncWebServer versions
- `WebServerImpl.cpp`: Implementation of the web server functionality
- `PerformanceRoutes.h/cpp`: Performance-related API endpoints
- `SecurityRoutes.h/cpp`: Security-related API endpoints

## Dependencies

- ESPAsyncWebServer-esphome (v3.1.0 or later)
- AsyncTCP-esphome (v2.1.1 or later)

## Configuration

The web server is configured through build flags in `platformio.ini`:

```ini
build_flags = 
    -DESP_ASYNC_WEBSERVER
    -DASYNC_TCP_SSL_ENABLED=1
    -DASYNC_TCP_PRIORITY=10
    -DWEBSERVER_TASK_PRIORITY=2
    -DWEBSERVER_STACK_SIZE=10240
```

## Usage

```cpp
#include "api/WebServer.h"

// Configure the server
WebServer::Config config;
config.port = 80;
config.hostname = "m5stack";

// Start the server
WebServer::begin(config);

// Add routes
WebServer::addRoute("/api/v1/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", "{\"status\":\"ok\"}");
});
```

## Error Handling

The server includes built-in error handling with standard HTTP status codes defined in the `WebServer::HttpCode` enum.

## Security Considerations

- SSL/TLS support is enabled by default
- Authentication should be implemented at the route level
- CORS headers should be configured as needed

## Contributing

When modifying the web server implementation:

1. Update the WebServer.h header if adding new functionality
2. Document new features in this README
3. Update security configurations if needed
4. Test all routes before committing