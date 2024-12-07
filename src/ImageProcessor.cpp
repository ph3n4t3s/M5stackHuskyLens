#include "ImageProcessor.h"
#include <algorithm>
#include <SPIFFS.h>
#include <ArduinoJson.h>

ImageProcessor::ImageProcessor()
    : autoEnhance(false),
      motionStabilization(false),
      denoiseLevel(0) {
    setupDefaultFilters();
}

bool ImageProcessor::begin() {
    return SPIFFS.begin(true);
}

void ImageProcessor::setupDefaultFilters() {
    // Filtre de flou gaussien
    const float gaussianKernel[3][3] = {
        {1/16.0f, 2/16.0f, 1/16.0f},
        {2/16.0f, 4/16.0f, 2/16.0f},
        {1/16.0f, 2/16.0f, 1/16.0f}
    };
    addCustomFilter("gaussian", (const float*)gaussianKernel);

    // Filtre de détection des bords (Sobel horizontal)
    const float sobelXKernel[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    addCustomFilter("sobel_x", (const float*)sobelXKernel);

    // Filtre de détection des bords (Sobel vertical)
    const float sobelYKernel[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };
    addCustomFilter("sobel_y", (const float*)sobelYKernel);

    // Filtre de netteté
    const float sharpenKernel[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };
    addCustomFilter("sharpen", (const float*)sharpenKernel);
}

void ImageProcessor::addCustomFilter(const String& name, const float kernel[3][3],
                                  float factor, float bias) {
    filters[name] = ImageFilter(name, kernel, factor, bias);
}

void ImageProcessor::removeFilter(const String& name) {
    filters.erase(name);
}

void ImageProcessor::processImage(SensorData& data) {
    if (data.points.empty()) return;

    // 1. Prétraitement
    if (currentEnhancement.nightMode) {
        enhanceNightVision();
    }

    if (motionStabilization) {
        stabilizeMotion();
    }

    if (denoiseLevel > 0) {
        denoise();
    }

    // 2. Ajustements basiques
    if (currentEnhancement.brightness != 1.0f) {
        adjustBrightness(currentEnhancement.brightness);
    }

    if (currentEnhancement.contrast != 1.0f) {
        adjustContrast(currentEnhancement.contrast);
    }

    if (currentEnhancement.saturation != 1.0f) {
        adjustSaturation(currentEnhancement.saturation);
    }

    if (currentEnhancement.sharpness != 1.0f) {
        applySharpening(currentEnhancement.sharpness);
    }

    // 3. Auto-amélioration si activée
    if (autoEnhance) {
        autoAdjustParameters();
    }

    // 4. Mise à jour des données
    updateDetectedFeatures(data);
}

void ImageProcessor::setEnhancement(const ImageEnhancement& enhancement) {
    currentEnhancement = enhancement;
}

void ImageProcessor::enableNightMode(bool enable) {
    currentEnhancement.nightMode = enable;
}

void ImageProcessor::setAutoEnhance(bool enable) {
    autoEnhance = enable;
}

void ImageProcessor::applyMotionStabilization(bool enable) {
    motionStabilization = enable;
}

void ImageProcessor::setDenoiseLevel(int level) {
    denoiseLevel = std::max(0, std::min(5, level));
}

float ImageProcessor::calculateBlurriness() {
    return calculateLaplacianVariance();
}

float ImageProcessor::calculateBrightness() {
    if (processedImage.empty()) return 0.0f;
    
    float sum = 0.0f;
    for (uint8_t pixel : processedImage) {
        sum += pixel;
    }
    return sum / processedImage.size();
}

std::vector<Point> ImageProcessor::detectEdges() {
    std::vector<Point> edges;
    
    // Appliquer Sobel X et Y
    applyFilter("sobel_x");
    std::vector<uint8_t> gradientX = processedImage;
    
    applyFilter("sobel_y");
    std::vector<uint8_t> gradientY = processedImage;
    
    // Calculer le gradient total
    const float threshold = 100.0f;
    for (size_t i = 0; i < gradientX.size(); i++) {
        float magnitude = sqrt(gradientX[i] * gradientX[i] + 
                             gradientY[i] * gradientY[i]);
        if (magnitude > threshold) {
            int x = i % 320;
            int y = i / 320;
            edges.push_back(Point(x, y));
        }
    }
    
    return edges;
}

std::vector<std::vector<Point>> ImageProcessor::detectContours() {
    std::vector<std::vector<Point>> contours;
    std::vector<Point> edges = detectEdges();
    
    // Regrouper les points connectés
    while (!edges.empty()) {
        std::vector<Point> contour;
        Point start = edges.back();
        edges.pop_back();
        contour.push_back(start);
        
        bool found;
        do {
            found = false;
            Point current = contour.back();
            
            for (auto it = edges.begin(); it != edges.end(); ) {
                if (isAdjacent(current, *it)) {
                    contour.push_back(*it);
                    it = edges.erase(it);
                    found = true;
                    break;
                } else {
                    ++it;
                }
            }
        } while (found);
        
        if (contour.size() > 5) { // Filtrer les petits contours
            contours.push_back(contour);
        }
    }
    
    return contours;
}

void ImageProcessor::denoise() {
    if (processedImage.empty() || denoiseLevel == 0) return;
    
    std::vector<uint8_t> temp = processedImage;
    const int size = 2 * denoiseLevel + 1;
    const int halfSize = denoiseLevel;
    
    for (int y = halfSize; y < 240 - halfSize; y++) {
        for (int x = halfSize; x < 320 - halfSize; x++) {
            std::vector<uint8_t> neighbors;
            
            // Collecter les pixels voisins
            for (int dy = -halfSize; dy <= halfSize; dy++) {
                for (int dx = -halfSize; dx <= halfSize; dx++) {
                    int idx = (y + dy) * 320 + (x + dx);
                    neighbors.push_back(processedImage[idx]);
                }
            }
            
            // Appliquer le filtre médian
            std::sort(neighbors.begin(), neighbors.end());
            temp[y * 320 + x] = neighbors[neighbors.size() / 2];
        }
    }
    
    processedImage = temp;
}

void ImageProcessor::stabilizeMotion() {
    if (!motionStabilization || processedImage.empty()) return;
    
    static std::vector<Point> lastCorners;
    std::vector<Point> currentCorners = detectHarrisCorners();
    
    if (!lastCorners.empty() && currentCorners.size() == lastCorners.size()) {
        // Calculer le déplacement moyen
        float dx = 0, dy = 0;
        int count = 0;
        
        for (size_t i = 0; i < currentCorners.size(); i++) {
            if (isValidCornerPair(currentCorners[i], lastCorners[i])) {
                dx += currentCorners[i].x - lastCorners[i].x;
                dy += currentCorners[i].y - lastCorners[i].y;
                count++;
            }
        }
        
        if (count > 0) {
            dx /= count;
            dy /= count;
            
            // Appliquer la correction
            std::vector<uint8_t> stabilized(processedImage.size(), 0);
            for (int y = 0; y < 240; y++) {
                for (int x = 0; x < 320; x++) {
                    int newX = x - round(dx);
                    int newY = y - round(dy);
                    
                    if (newX >= 0 && newX < 320 && newY >= 0 && newY < 240) {
                        stabilized[y * 320 + x] = processedImage[newY * 320 + newX];
                    }
                }
            }
            processedImage = stabilized;
        }
    }
    
    lastCorners = currentCorners;
}

void ImageProcessor::enhanceNightVision() {
    if (!currentEnhancement.nightMode || processedImage.empty()) return;
    
    // Conversion en YUV pour séparer la luminance
    convertToYUV();
    
    // Améliorer la luminance
    for (size_t i = 0; i < processedImage.size(); i += 3) {
        float Y = processedImage[i];
        
        // Courbe de réponse logarithmique
        Y = 255.0f * log(1 + Y) / log(256);
        
        // Réduction du bruit dans les zones sombres
        if (Y < 50) {
            Y = Y * 1.5f;
        }
        
        processedImage[i] = std::min(255.0f, Y);
    }
    
    // Retour en RGB
    convertToRGB();
}

void ImageProcessor::autoAdjustParameters() {
    float brightness = calculateBrightness();
    float blurriness = calculateBlurriness();
    
    // Ajuster la luminosité
    if (brightness < 100) {
        currentEnhancement.brightness *= 1.1f;
    } else if (brightness > 200) {
        currentEnhancement.brightness *= 0.9f;
    }
    
    // Ajuster la netteté
    if (blurriness > 100) {
        currentEnhancement.sharpness *= 1.1f;
    }
    
    // Limiter les valeurs
    currentEnhancement.brightness = std::max(0.5f, std::min(2.0f, currentEnhancement.brightness));
    currentEnhancement.sharpness = std::max(0.5f, std::min(2.0f, currentEnhancement.sharpness));
}

void ImageProcessor::saveSettings(const String& filename) {
    File file = SPIFFS.open(filename, "w");
    if (!file) return;
    
    DynamicJsonDocument doc(1024);
    
    doc["brightness"] = currentEnhancement.brightness;
    doc["contrast"] = currentEnhancement.contrast;
    doc["saturation"] = currentEnhancement.saturation;
    doc["sharpness"] = currentEnhancement.sharpness;
    doc["nightMode"] = currentEnhancement.nightMode;
    doc["autoEnhance"] = autoEnhance;
    doc["motionStabilization"] = motionStabilization;
    doc["denoiseLevel"] = denoiseLevel;
    
    serializeJson(doc, file);
    file.close();
}

bool ImageProcessor::loadSettings(const String& filename) {
    File file = SPIFFS.open(filename, "r");
    if (!file) return false;
    
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    
    if (error) return false;
    
    currentEnhancement.brightness = doc["brightness"] | 1.0f;
    currentEnhancement.contrast = doc["contrast"] | 1.0f;
    currentEnhancement.saturation = doc["saturation"] | 1.0f;
    currentEnhancement.sharpness = doc["sharpness"] | 1.0f;
    currentEnhancement.nightMode = doc["nightMode"] | false;
    autoEnhance = doc["autoEnhance"] | false;
    motionStabilization = doc["motionStabilization"] | false;
    denoiseLevel = doc["denoiseLevel"] | 0;
    
    return true;
}

std::vector<String> ImageProcessor::getAvailableFilters() const {
    std::vector<String> filterNames;
    for (const auto& filter : filters) {
        filterNames.push_back(filter.first);
    }
    return filterNames;
}