#pragma once

#include <M5CoreS3.h>
#include <unity.h>

class TestBase {
protected:
    static bool hardwareInitialized;
    
    static void initHardware() {
        if (!hardwareInitialized) {
            M5.begin();
            Serial.begin(115200);
            delay(2000); // Attendre que le port série soit prêt
            hardwareInitialized = true;
        }
    }
    
    static void printTestInfo(const char* testName) {
        Serial.printf("\n--- Starting Test: %s ---\n", testName);
    }
    
    static void checkMemory(const char* point) {
        Serial.printf("Memory at %s: %d bytes free\n", 
                     point, ESP.getFreeHeap());
    }
};

bool TestBase::hardwareInitialized = false;