#pragma once

#include <vector>
#include <algorithm>
#include <cmath>
#include "Config.h"

// Fonctions utilitaires pour la géométrie
namespace GeometryUtils {

inline int minX(const std::vector<Point>& points) {
    if (points.empty()) return 0;
    return std::min_element(points.begin(), points.end(),
        [](const Point& a, const Point& b) { return a.x < b.x; })->x;
}

inline int maxX(const std::vector<Point>& points) {
    if (points.empty()) return 0;
    return std::max_element(points.begin(), points.end(),
        [](const Point& a, const Point& b) { return a.x < b.x; })->x;
}

inline int minY(const std::vector<Point>& points) {
    if (points.empty()) return 0;
    return std::min_element(points.begin(), points.end(),
        [](const Point& a, const Point& b) { return a.y < b.y; })->y;
}

inline int maxY(const std::vector<Point>& points) {
    if (points.empty()) return 0;
    return std::max_element(points.begin(), points.end(),
        [](const Point& a, const Point& b) { return a.y < b.y; })->y;
}

inline Point calculateCentroid(const std::vector<Point>& points) {
    if (points.empty()) return Point(0, 0);
    
    int sumX = 0, sumY = 0;
    for (const auto& p : points) {
        sumX += p.x;
        sumY += p.y;
    }
    return Point(sumX / points.size(), sumY / points.size());
}

inline float calculateCircularity(const std::vector<Point>& points) {
    if (points.size() < 3) return 0.0f;
    
    // Calculer le périmètre
    float perimeter = 0;
    for (size_t i = 0; i < points.size(); i++) {
        const Point& p1 = points[i];
        const Point& p2 = points[(i + 1) % points.size()];
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        perimeter += sqrt(dx * dx + dy * dy);
    }
    
    // Calculer l'aire avec la formule du shoelace
    float area = 0;
    for (size_t i = 0; i < points.size(); i++) {
        const Point& p1 = points[i];
        const Point& p2 = points[(i + 1) % points.size()];
        area += (p1.x * p2.y - p2.x * p1.y);
    }
    area = abs(area) / 2;
    
    // Calculer la circularité avec la formule 4πA/P²
    return 4 * M_PI * area / (perimeter * perimeter);
}

inline float calculateCompactness(const std::vector<Point>& points) {
    if (points.size() < 3) return 0.0f;
    
    int minx = minX(points), maxx = maxX(points);
    int miny = minY(points), maxy = maxY(points);
    float boundingArea = (maxx - minx) * (maxy - miny);
    
    // Calculer l'aire de la forme
    float area = 0;
    for (size_t i = 0; i < points.size(); i++) {
        const Point& p1 = points[i];
        const Point& p2 = points[(i + 1) % points.size()];
        area += (p1.x * p2.y - p2.x * p1.y);
    }
    area = abs(area) / 2;
    
    return area / boundingArea;
}

inline float calculateAspectRatio(const std::vector<Point>& points) {
    if (points.size() < 2) return 1.0f;
    
    int minx = minX(points), maxx = maxX(points);
    int miny = minY(points), maxy = maxY(points);
    float width = maxx - minx;
    float height = maxy - miny;
    
    if (height == 0) return 1.0f;
    return width / height;
}

inline std::vector<float> calculateHuMoments(const std::vector<Point>& points, const Point& centroid) {
    std::vector<float> moments(7, 0.0f);
    if (points.size() < 3) return moments;
    
    // Calculer les moments centraux
    float m00 = points.size();
    float mu20 = 0, mu02 = 0, mu11 = 0;
    float mu30 = 0, mu03 = 0, mu12 = 0, mu21 = 0;
    
    for (const auto& p : points) {
        float x = p.x - centroid.x;
        float y = p.y - centroid.y;
        float x2 = x * x;
        float y2 = y * y;
        float x3 = x2 * x;
        float y3 = y2 * y;
        
        mu20 += x2;
        mu02 += y2;
        mu11 += x * y;
        mu30 += x3;
        mu03 += y3;
        mu12 += x * y2;
        mu21 += x2 * y;
    }
    
    // Normaliser
    float inv_m00 = 1.0f / m00;
    mu20 *= inv_m00;
    mu02 *= inv_m00;
    mu11 *= inv_m00;
    mu30 *= inv_m00;
    mu03 *= inv_m00;
    mu12 *= inv_m00;
    mu21 *= inv_m00;
    
    // Calculer les moments invariants de Hu
    moments[0] = mu20 + mu02;
    moments[1] = (mu20 - mu02) * (mu20 - mu02) + 4 * mu11 * mu11;
    moments[2] = (mu30 - 3 * mu12) * (mu30 - 3 * mu12) + 
                 (3 * mu21 - mu03) * (3 * mu21 - mu03);
    moments[3] = (mu30 + mu12) * (mu30 + mu12) + 
                 (mu21 + mu03) * (mu21 + mu03);
    moments[4] = (mu30 - 3 * mu12) * (mu30 + mu12) * 
                 ((mu30 + mu12) * (mu30 + mu12) - 3 * (mu21 + mu03) * (mu21 + mu03)) +
                 (3 * mu21 - mu03) * (mu21 + mu03) * 
                 (3 * (mu30 + mu12) * (mu30 + mu12) - (mu21 + mu03) * (mu21 + mu03));
    moments[5] = (mu20 - mu02) * ((mu30 + mu12) * (mu30 + mu12) - 
                 (mu21 + mu03) * (mu21 + mu03)) + 
                 4 * mu11 * (mu30 + mu12) * (mu21 + mu03);
    moments[6] = (3 * mu21 - mu03) * (mu30 + mu12) * 
                 ((mu30 + mu12) * (mu30 + mu12) - 3 * (mu21 + mu03) * (mu21 + mu03)) -
                 (mu30 - 3 * mu12) * (mu21 + mu03) * 
                 (3 * (mu30 + mu12) * (mu30 + mu12) - (mu21 + mu03) * (mu21 + mu03));
    
    return moments;
}

} // namespace GeometryUtils