#pragma once

#include <vector>
#include <deque>
#include "Config.h"

struct GesturePattern {
    String name;
    std::vector<Point> pattern;
    float tolerance;
    
    GesturePattern(const String& n, const std::vector<Point>& p, float t = 0.2f)
        : name(n), pattern(p), tolerance(t) {}
};

class GestureAnalyzer {
public:
    GestureAnalyzer();
    void addPattern(const String& name, const std::vector<Point>& pattern, float tolerance = 0.2f);
    void addPoint(const Point& point);
    void clearPoints();
    String recognizeGesture();
    float getConfidence() const;
    void setMaxPoints(size_t max);
    void setTimeout(unsigned long timeout);
    bool isGestureInProgress() const;
    
private:
    std::deque<Point> points;
    std::vector<GesturePattern> patterns;
    size_t maxPoints;
    unsigned long gestureTimeout;
    unsigned long lastPointTime;
    float lastConfidence;
    
    float comparePatterns(const std::vector<Point>& p1, const std::vector<Point>& p2) const;
    std::vector<Point> normalizePattern(const std::vector<Point>& pattern) const;
    float calculateDTW(const std::vector<Point>& p1, const std::vector<Point>& p2) const;
    void resamplePoints(std::vector<Point>& points, int numPoints) const;
    Point centeroid(const std::vector<Point>& points) const;
    void scaleToUnit(std::vector<Point>& points) const;
    float pathLength(const std::vector<Point>& points) const;
};