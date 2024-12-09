#pragma once

#ifdef ARDUINO_ARCH_ESP32
#if defined(ESP_ASYNC_WEBSERVER)
#include <ESPAsyncWebServer.h>
#else 
#include <AsyncWebServer_ESPAsyncWebServer.h>
#endif
#else
#include <ESPAsyncWebServer.h>
#endif