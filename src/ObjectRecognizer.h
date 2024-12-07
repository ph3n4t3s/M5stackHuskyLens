#pragma once

#include <vector>
#include <map>
#include "Config.h"
#include <ArduinoJson.h>

struct ObjectTemplate {
    String name;
    std::vector<Point> contour;
    float width;
    float height;
    float aspectRatio;
    std::vector<float> features;
    
    ObjectTemplate() : width(0), height(0), aspectRatio(0) {}
    ObjectTemplate(const String& n, const std::vector<Point>& c);
};

struct ObjectMatch {
    String name;
    float confidence;
    Point position;
    float width;
    float height;
    float rotation;
    
    ObjectMatch() : confidence(0), width(0), height(0), rotation(0) {}
};

class ObjectRecognizer {
public:
    ObjectRecognizer();
    bool begin();
    void addTemplate(const String& name, const std::vector<Point>& contour);
    void removeTemplate(const String& name);
    std::vector<ObjectMatch> recognizeObjects(const std::vector<Point>& points);
    bool saveTemplates(const String& filename);
    bool loadTemplates(const String& filename);
    void setMinConfidence(float confidence);
    void enableRotationInvariant(bool enable);
    void enableScaleInvariant(bool enable);
    std::vector<String> getTemplateNames() const;
    
private:
    std::map<String, ObjectTemplate> templates;
    float minConfidence;
    bool rotationInvariant;
    bool scaleInvariant;
    
    std::vector<float> extractFeatures(const std::vector<Point>& points);
    float compareFeatures(const std::vector<float>& f1, const std::vector<float>& f2);
    std::vector<Point> normalizeContour(const std::vector<Point>& points);
    float calculateIOU(const std::vector<Point>& p1, const std::vector<Point>& p2);
    ObjectTemplate createTemplate(const String& name, const std::vector<Point>& contour);
    std::vector<Point> rotatePoints(const std::vector<Point>& points, float angle);
    std::vector<Point> scalePoints(const std::vector<Point>& points, float scale);
    float findBestRotation(const std::vector<Point>& p1, const std::vector<Point>& p2);
};