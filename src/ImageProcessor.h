#pragma once

#include <vector>
#include <map>
#include <algorithm>
#include "SensorData.h"
#include "Types.h"
#include "Constants.h"
#include <FastLED.h>

// Les filtres sont stockés comme des matrices 3x3
struct ImageFilter {
    String name;
    float kernel[3][3];
    float factor; // Facteur de multiplication final
    float bias;   // Valeur à ajouter aux résultats
    
    ImageFilter() : name(""), factor(1.0f), bias(0.0f) {
        // Initialiser à un filtre identité par défaut
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                kernel[i][j] = (i == j) ? 1.0f : 0.0f;
    }
    
    ImageFilter(const String& n, const float k[3][3], float f = 1.0f, float b = 0.0f)
        : name(n), factor(f), bias(b) {
        memcpy(kernel, k, 9 * sizeof(float));
    }
};

class ImageProcessor {
public:
    bool begin();
    
    // Filtres et effets
    void applyFilter(const String& filterName);
    void addCustomFilter(const String& name, const float kernel[3][3],
                        float factor = 1.0f, float bias = 0.0f);
    void removeFilter(const String& name);
    
    // Configuration
    void setDenoiseLevel(int level);
    void applyMotionStabilization(bool enable);
    std::vector<String> getAvailableFilters() const;
    std::vector<Point> detectHarrisCorners() const;
    
    // Traitement principal
    void processImage(SensorData& data);

private:
    int denoiseLevel;
    bool motionStabilization;
    std::map<String, ImageFilter> filters;
    std::vector<uint8_t> processedImage;
    
    // Filtres prédéfinis
    void setupDefaultFilters();
    void applyKernel(const float kernel[3][3], float factor, float bias);
    void stabilizeMotion();
    void denoise();

    // Utilitaires de traitement d'image
    float calculateBlurriness() const;
    std::vector<Point> detectEdges() const;
    std::vector<std::vector<Point>> detectContours() const;
};