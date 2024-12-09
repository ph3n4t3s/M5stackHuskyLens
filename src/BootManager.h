#pragma once

#include "display/DisplayManager.h"
#include "Types.h"
#include <vector>

class BootManager {
public:
    BootManager(DisplayManager& display);
    
    void begin();
    void update();
    bool isBootComplete() const;
    void showBootSequence(const String& message);
    
private:
    DisplayManager& displayManager;
    bool bootComplete;
    unsigned long bootStartTime;
    std::vector<BootMessage> bootMessages;
    
    void initializeHardware();
    void checkDependencies();
    void finalizeBootSequence();
};