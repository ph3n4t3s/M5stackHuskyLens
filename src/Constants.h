#pragma once

namespace Constants {
    namespace Display {
        const uint8_t DEFAULT_BRIGHTNESS = 128;
        const int DEFAULT_ROTATION = 0;
        const int SCREEN_WIDTH = 320;
        const int SCREEN_HEIGHT = 240;
    }

    namespace Colors {
        const uint32_t BACKGROUND = 0x000000;  // Black
        const uint32_t TEXT = 0xFFFFFF;        // White
        const uint32_t ERROR = 0xFF0000;       // Red
        const uint32_t SUCCESS = 0x00FF00;     // Green
        const uint32_t WARNING = 0xFFFF00;     // Yellow
        const uint32_t INFO = 0x0000FF;        // Blue
    }

    namespace History {  
        const int MAX_HISTORY = 100;
    }
}