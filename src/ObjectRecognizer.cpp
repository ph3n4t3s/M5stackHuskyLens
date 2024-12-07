#include "ObjectRecognizer.h"
#include "GeometryUtils.h"
#include <cmath>
#include <algorithm>
#include <SPIFFS.h>

using namespace GeometryUtils;

ObjectTemplate::ObjectTemplate(const String& n, const std::vector<Point>& c)
    : name(n), contour(c) {
    // Calculer les dimensions et le ratio
    int minX = INT_MAX, minY = INT_MAX;
    int maxX = INT_MIN, maxY = INT_MIN;
    
    for (const auto& p : contour) {
        minX = std::min(minX, p.x);
        minY = std::min(minY, p.y);
        maxX = std::max(maxX, p.x);
        maxY = std::max(maxY, p.y);
    }
    
    width = maxX - minX;
    height = maxY - minY;
    aspectRatio = width / height;
}

ObjectRecognizer::ObjectRecognizer()
    : minConfidence(0.7f),
      rotationInvariant(true),
      scaleInvariant(true) {}

bool ObjectRecognizer::begin() {
    return SPIFFS.begin(true);
}

void ObjectRecognizer::addTemplate(const String& name, const std::vector<Point>& contour) {
    templates[name] = createTemplate(name, contour);
}

void ObjectRecognizer::removeTemplate(const String& name) {
    templates.erase(name);
}

std::vector<ObjectMatch> ObjectRecognizer::recognizeObjects(const std::vector<Point>& points) {
    std::vector<ObjectMatch> matches;
    if (points.size() < 3) return matches;
    
    std::vector<float> inputFeatures = extractFeatures(points);
    std::vector<Point> normalizedInput = normalizeContour(points);
    
    for (const auto& templ : templates) {
        ObjectMatch match;
        match.name = templ.first;
        
        // Comparer les caractéristiques
        float featureConfidence = compareFeatures(inputFeatures, templ.second.features);
        
        // Comparer les contours
        float bestIOU = 0.0f;
        float bestRotation = 0.0f;
        std::vector<Point> bestPoints = normalizedInput;
        
        if (rotationInvariant) {
            bestRotation = findBestRotation(normalizedInput, templ.second.contour);
            bestPoints = rotatePoints(normalizedInput, bestRotation);
        }
        
        if (scaleInvariant) {
            float scaleX = templ.second.width / (maxX(bestPoints) - minX(bestPoints));
            float scaleY = templ.second.height / (maxY(bestPoints) - minY(bestPoints));
            float scale = (scaleX + scaleY) / 2.0f;
            bestPoints = scalePoints(bestPoints, scale);
        }
        
        bestIOU = calculateIOU(bestPoints, templ.second.contour);
        
        // Calculer la confiance finale
        match.confidence = (featureConfidence + bestIOU) / 2.0f;
        
        if (match.confidence >= minConfidence) {
            // Calculer la position et les dimensions
            match.position = calculateCentroid(points);
            match.width = maxX(points) - minX(points);
            match.height = maxY(points) - minY(points);
            match.rotation = bestRotation;
            matches.push_back(match);
        }
    }
    
    // Trier par confiance
    std::sort(matches.begin(), matches.end(),
        [](const ObjectMatch& a, const ObjectMatch& b) {
            return a.confidence > b.confidence;
        });
    
    return matches;
}

bool ObjectRecognizer::saveTemplates(const String& filename) {
    File file = SPIFFS.open(filename, "w");
    if (!file) return false;
    
    DynamicJsonDocument doc(16384);
    JsonArray templatesArray = doc.createNestedArray("templates");
    
    for (const auto& templ : templates) {
        JsonObject obj = templatesArray.createNestedObject();
        obj["name"] = templ.first;
        
        JsonArray contour = obj.createNestedArray("contour");
        for (const auto& p : templ.second.contour) {
            JsonObject point = contour.createNestedObject();
            point["x"] = p.x;
            point["y"] = p.y;
        }
        
        JsonArray features = obj.createNestedArray("features");
        for (float f : templ.second.features) {
            features.add(f);
        }
        
        obj["width"] = templ.second.width;
        obj["height"] = templ.second.height;
        obj["aspectRatio"] = templ.second.aspectRatio;
    }
    
    serializeJson(doc, file);
    file.close();
    return true;
}

bool ObjectRecognizer::loadTemplates(const String& filename) {
    File file = SPIFFS.open(filename, "r");
    if (!file) return false;
    
    DynamicJsonDocument doc(16384);
    DeserializationError error = deserializeJson(doc, file);
    if (error) return false;
    
    templates.clear();
    JsonArray templatesArray = doc["templates"];
    
    for (JsonObject templObj : templatesArray) {
        String name = templObj["name"].as<String>();
        std::vector<Point> contour;
        
        JsonArray contourArray = templObj["contour"];
        for (JsonObject point : contourArray) {
            contour.push_back(Point(
                point["x"].as<int>(),
                point["y"].as<int>()
            ));
        }
        
        ObjectTemplate templ(name, contour);
        
        JsonArray featuresArray = templObj["features"];
        for (float f : featuresArray) {
            templ.features.push_back(f);
        }
        
        templates[name] = templ;
    }
    
    file.close();
    return true;
}

void ObjectRecognizer::setMinConfidence(float confidence) {
    minConfidence = std::max(0.0f, std::min(1.0f, confidence));
}

void ObjectRecognizer::enableRotationInvariant(bool enable) {
    rotationInvariant = enable;
}

void ObjectRecognizer::enableScaleInvariant(bool enable) {
    scaleInvariant = enable;
}

std::vector<String> ObjectRecognizer::getTemplateNames() const {
    std::vector<String> names;
    for (const auto& templ : templates) {
        names.push_back(templ.first);
    }
    return names;
}

std::vector<float> ObjectRecognizer::extractFeatures(const std::vector<Point>& points) {
    std::vector<float> features;
    
    // Calculer le centre de masse
    Point centroid = calculateCentroid(points);
    
    // Caractéristiques de forme
    features.push_back(calculateCircularity(points));
    features.push_back(calculateCompactness(points));
    features.push_back(calculateAspectRatio(points));
    
    // Moments invariants de Hu
    std::vector<float> huMoments = calculateHuMoments(points, centroid);
    features.insert(features.end(), huMoments.begin(), huMoments.end());
    
    return features;
}

float ObjectRecognizer::compareFeatures(const std::vector<float>& f1, const std::vector<float>& f2) {
    if (f1.size() != f2.size()) return 0.0f;
    
    float sumSquaredDiff = 0.0f;
    for (size_t i = 0; i < f1.size(); i++) {
        float diff = f1[i] - f2[i];
        sumSquaredDiff += diff * diff;
    }
    
    return 1.0f / (1.0f + sqrt(sumSquaredDiff));
}

std::vector<Point> ObjectRecognizer::normalizeContour(const std::vector<Point>& points) {
    Point centroid = calculateCentroid(points);
    std::vector<Point> normalized;
    
    // Centrer les points
    for (const auto& p : points) {
        normalized.push_back(Point(p.x - centroid.x, p.y - centroid.y));
    }
    
    // Normaliser l'échelle
    float maxDist = 0.0f;
    for (const auto& p : normalized) {
        float dist = sqrt(p.x * p.x + p.y * p.y);
        maxDist = std::max(maxDist, dist);
    }
    
    if (maxDist > 0) {
        for (auto& p : normalized) {
            p.x = (p.x * 100) / maxDist;
            p.y = (p.y * 100) / maxDist;
        }
    }
    
    return normalized;
}

float ObjectRecognizer::calculateIOU(const std::vector<Point>& p1, const std::vector<Point>& p2) {
    // Implémentation simplifiée basée sur les boîtes englobantes
    int minX1 = INT_MAX, minY1 = INT_MAX, maxX1 = INT_MIN, maxY1 = INT_MIN;
    int minX2 = INT_MAX, minY2 = INT_MAX, maxX2 = INT_MIN, maxY2 = INT_MIN;
    
    for (const auto& p : p1) {
        minX1 = std::min(minX1, p.x);
        minY1 = std::min(minY1, p.y);
        maxX1 = std::max(maxX1, p.x);
        maxY1 = std::max(maxY1, p.y);
    }
    
    for (const auto& p : p2) {
        minX2 = std::min(minX2, p.x);
        minY2 = std::min(minY2, p.y);
        maxX2 = std::max(maxX2, p.x);
        maxY2 = std::max(maxY2, p.y);
    }
    
    int intersectMinX = std::max(minX1, minX2);
    int intersectMinY = std::max(minY1, minY2);
    int intersectMaxX = std::min(maxX1, maxX2);
    int intersectMaxY = std::min(maxY1, maxY2);
    
    if (intersectMaxX < intersectMinX || intersectMaxY < intersectMinY) {
        return 0.0f;
    }
    
    float intersectArea = (intersectMaxX - intersectMinX) * (intersectMaxY - intersectMinY);
    float area1 = (maxX1 - minX1) * (maxY1 - minY1);
    float area2 = (maxX2 - minX2) * (maxY2 - minY2);
    float unionArea = area1 + area2 - intersectArea;
    
    return intersectArea / unionArea;
}

ObjectTemplate ObjectRecognizer::createTemplate(const String& name, const std::vector<Point>& contour) {
    ObjectTemplate templ(name, normalizeContour(contour));
    templ.features = extractFeatures(contour);
    return templ;
}

std::vector<Point> ObjectRecognizer::rotatePoints(const std::vector<Point>& points, float angle) {
    std::vector<Point> rotated;
    float cosA = cos(angle);
    float sinA = sin(angle);
    
    for (const auto& p : points) {
        int x = round(p.x * cosA - p.y * sinA);
        int y = round(p.x * sinA + p.y * cosA);
        rotated.push_back(Point(x, y));
    }
    
    return rotated;
}

std::vector<Point> ObjectRecognizer::scalePoints(const std::vector<Point>& points, float scale) {
    std::vector<Point> scaled;
    for (const auto& p : points) {
        scaled.push_back(Point(round(p.x * scale), round(p.y * scale)));
    }
    return scaled;
}

float ObjectRecognizer::findBestRotation(const std::vector<Point>& p1, const std::vector<Point>& p2) {
    float bestAngle = 0.0f;
    float bestIOU = 0.0f;
    
    for (float angle = 0; angle < 2 * M_PI; angle += M_PI / 36) {
        std::vector<Point> rotated = rotatePoints(p1, angle);
        float iou = calculateIOU(rotated, p2);
        if (iou > bestIOU) {
            bestIOU = iou;
            bestAngle = angle;
        }
    }
    
    return bestAngle;
}