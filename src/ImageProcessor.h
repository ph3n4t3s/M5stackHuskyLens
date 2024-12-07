#pragma once

#include <vector>
#include "Config.h"
#include <FastLED.h>

struct ImageFilter {
    String name;
    float kernel[3][3];
    float factor;
    float bias;
    
    ImageFilter(const String& n, const float k[3][3], float f = 1.0f, float b = 0.0f)
        : name(n), factor(f), bias(b) {
        memcpy(kernel, k, 9 * sizeof(float));
    }
};

struct ImageEnhancement {
    float brightness;
    float contrast;
    float saturation;
    float sharpness;
    bool nightMode;
    
    ImageEnhancement()
        : brightness(1.0f), contrast(1.0f), saturation(1.0f),
          sharpness(1.0f), nightMode(false) {}
};

class ImageProcessor {
public:
    ImageProcessor();
    bool begin();
    
    // Filtres et effets
    void applyFilter(const String& filterName);
    void addCustomFilter(const String& name, const float kernel[3][3],
                        float factor = 1.0f, float bias = 0.0f);
    void removeFilter(const String& name);
    
    // Améliorations d'image
    void setEnhancement(const ImageEnhancement& enhancement);
    void enableNightMode(bool enable);
    void setAutoEnhance(bool enable);
    
    // Traitement d'image
    void processImage(SensorData& data);
    void applyMotionStabilization(bool enable);
    void setDenoiseLevel(int level);
    
    // Analyse d'image
    float calculateBlurriness();
    float calculateBrightness();
    std::vector<Point> detectEdges();
    std::vector<std::vector<Point>> detectContours();
    
    // Configuration
    void saveSettings(const String& filename);
    bool loadSettings(const String& filename);
    std::vector<String> getAvailableFilters() const;
    
private:
    std::map<String, ImageFilter> filters;
    ImageEnhancement currentEnhancement;
    bool autoEnhance;
    bool motionStabilization;
    int denoiseLevel;
    
    // Filtres prédéfinis
    void setupDefaultFilters();
    std::vector<uint8_t> originalImage;
    std::vector<uint8_t> processedImage;
    
    // Fonctions de traitement
    void applyKernel(const float kernel[3][3], float factor, float bias);
    void adjustBrightness(float value);
    void adjustContrast(float value);
    void adjustSaturation(float value);
    void applySharpening(float amount);
    void denoise();
    void stabilizeMotion();
    void enhanceNightVision();
    void autoAdjustParameters();
    
    // Utilitaires
    void updateHistogram();
    float calculateLaplacianVariance();
    std::vector<Point> detectHarrisCorners();
    void convertToYUV();
    void convertToRGB();
};