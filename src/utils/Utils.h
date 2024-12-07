#pragma once

#include <vector>
#include <algorithm>
#include "../Config.h"

namespace Utils {
    // Fonctions de géométrie
    Point calculateCentroid(const std::vector<Point>& points);
    float calculateCircularity(const std::vector<Point>& points);
    float calculateCompactness(const std::vector<Point>& points);
    float calculateAspectRatio(const std::vector<Point>& points);
    std::vector<float> calculateHuMoments(const std::vector<Point>& points,
                                        const Point& centroid);
    
    // Fonctions de calcul
    float minX(const std::vector<Point>& points);
    float maxX(const std::vector<Point>& points);
    float minY(const std::vector<Point>& points);
    float maxY(const std::vector<Point>& points);
    bool isAdjacent(const Point& p1, const Point& p2, float threshold = 5.0f);
    bool isValidCornerPair(const Point& p1, const Point& p2, float threshold = 50.0f);
    
    // Fonctions mathématiques
    template<typename T>
    T clamp(T value, T min, T max) {
        return std::max(min, std::min(max, value));
    }
    
    float distance(const Point& p1, const Point& p2);
    float angle(const Point& p1, const Point& p2);
    
    // Fonctions de traitement d'image
    float calculateBrightness(const uint8_t* data, size_t size);
    float calculateContrast(const uint8_t* data, size_t size);
    float calculateBlur(const uint8_t* data, size_t width, size_t height);
    
    // Fonctions de conversion
    template<typename T>
    String toString(const T& value) {
        return String(value);
    }
    
    template<typename T>
    T fromString(const String& str) {
        if constexpr (std::is_same<T, int>::value) {
            return str.toInt();
        } else if constexpr (std::is_same<T, float>::value) {
            return str.toFloat();
        } else if constexpr (std::is_same<T, bool>::value) {
            return str.equalsIgnoreCase("true") || str == "1";
        } else {
            return str;
        }
    }
    
    // Fonctions de validation
    bool isNumeric(const String& str);
    bool isAlphanumeric(const String& str);
    bool isValidEmail(const String& str);
    bool isValidIp(const String& str);
    
    // Fonctions de temps
    String getCurrentTimestamp();
    String formatDuration(unsigned long milliseconds);
    String formatDateTime(time_t timestamp);
    
    // Fonctions système
    size_t getFreeHeap();
    size_t getMaxAllocHeap();
    float getCpuFrequency();
    float getCpuTemperature();
    String getChipId();
    
    // Fonctions de sécurité
    String generateRandomString(size_t length);
    String hashString(const String& input);
    bool verifyHash(const String& input, const String& hash);
}