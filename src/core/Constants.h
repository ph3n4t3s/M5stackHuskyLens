#pragma once

namespace Constants {
    // Tailles d'écran
    constexpr int SCREEN_WIDTH = 320;
    constexpr int SCREEN_HEIGHT = 240;
    
    // Paramètres d'affichage
    namespace Display {
        constexpr uint8_t DEFAULT_BRIGHTNESS = 128;  // 50%
        constexpr int DEFAULT_ROTATION = 0;          // Portrait
    }
    
    // Couleurs
    namespace Colors {
        constexpr uint32_t BACKGROUND = 0x0000;  // Noir
        constexpr uint32_t TEXT = 0xFFFF;        // Blanc
    }
}