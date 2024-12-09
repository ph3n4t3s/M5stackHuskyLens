#include <M5CoreS3.h>
#include "src/display/DisplayManager.h"

DisplayManager display;

void setup() {
    M5.begin();
    Serial.begin(115200);
    Serial.println("Démarrage...");
    
    display.begin();
    display.showMessage("M5Stack HuskyLens v2");
}

void loop() {
    M5.update();
    display.update();
    delay(10);
}