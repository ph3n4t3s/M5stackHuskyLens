#include "GestureAnalyzer.h"
#include <cmath>
#include <algorithm>

GestureAnalyzer::GestureAnalyzer() 
    : maxPoints(64), 
      gestureTimeout(1000), 
      lastPointTime(0),
      lastConfidence(0.0f) {
    
    // Ajouter quelques gestes prédéfinis
    std::vector<Point> circle;
    for (int i = 0; i < 32; i++) {
        float angle = i * 2 * M_PI / 32;
        circle.push_back(Point(cos(angle) * 100, sin(angle) * 100));
    }
    addPattern("circle", circle);
    
    std::vector<Point> square = {
        Point(0, 0),
        Point(100, 0),
        Point(100, 100),
        Point(0, 100),
        Point(0, 0)
    };
    addPattern("square", square);
    
    std::vector<Point> zigzag;
    for (int i = 0; i < 4; i++) {
        zigzag.push_back(Point(i * 50, (i % 2) * 100));
    }
    addPattern("zigzag", zigzag);
}

void GestureAnalyzer::addPattern(const String& name, const std::vector<Point>& pattern, float tolerance) {
    patterns.emplace_back(name, normalizePattern(pattern), tolerance);
}

void GestureAnalyzer::addPoint(const Point& point) {
    unsigned long now = millis();
    if (now - lastPointTime > gestureTimeout) {
        points.clear();
    }
    lastPointTime = now;
    
    points.push_back(point);
    if (points.size() > maxPoints) {
        points.pop_front();
    }
}

void GestureAnalyzer::clearPoints() {
    points.clear();
}

String GestureAnalyzer::recognizeGesture() {
    if (points.size() < 10) return "";
    
    std::vector<Point> current(points.begin(), points.end());
    std::vector<Point> normalized = normalizePattern(current);
    
    float bestMatch = 0.0f;
    String bestGesture = "";
    
    for (const auto& pattern : patterns) {
        float similarity = comparePatterns(normalized, pattern.pattern);
        if (similarity > bestMatch && similarity > pattern.tolerance) {
            bestMatch = similarity;
            bestGesture = pattern.name;
        }
    }
    
    lastConfidence = bestMatch;
    return bestGesture;
}

float GestureAnalyzer::getConfidence() const {
    return lastConfidence;
}

void GestureAnalyzer::setMaxPoints(size_t max) {
    maxPoints = max;
    while (points.size() > maxPoints) {
        points.pop_front();
    }
}

void GestureAnalyzer::setTimeout(unsigned long timeout) {
    gestureTimeout = timeout;
}

bool GestureAnalyzer::isGestureInProgress() const {
    return !points.empty() && (millis() - lastPointTime <= gestureTimeout);
}

float GestureAnalyzer::comparePatterns(const std::vector<Point>& p1, const std::vector<Point>& p2) const {
    float dtw = calculateDTW(p1, p2);
    float maxDistance = p1.size() * 2.0f; // Distance maximale possible
    return 1.0f - (dtw / maxDistance);
}

std::vector<Point> GestureAnalyzer::normalizePattern(const std::vector<Point>& pattern) const {
    std::vector<Point> normalized = pattern;
    resamplePoints(normalized, 32);
    Point center = centeroid(normalized);
    
    // Centrer le motif
    for (auto& p : normalized) {
        p.x -= center.x;
        p.y -= center.y;
    }
    
    scaleToUnit(normalized);
    return normalized;
}

float GestureAnalyzer::calculateDTW(const std::vector<Point>& p1, const std::vector<Point>& p2) const {
    const size_t n = p1.size();
    const size_t m = p2.size();
    std::vector<std::vector<float>> dtw(n + 1, std::vector<float>(m + 1, INFINITY));
    
    dtw[0][0] = 0;
    
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 1; j <= m; j++) {
            float cost = hypot(p1[i-1].x - p2[j-1].x, p1[i-1].y - p2[j-1].y);
            dtw[i][j] = cost + std::min({
                dtw[i-1][j],
                dtw[i][j-1],
                dtw[i-1][j-1]
            });
        }
    }
    
    return dtw[n][m];
}

void GestureAnalyzer::resamplePoints(std::vector<Point>& points, int numPoints) const {
    float pathLen = pathLength(points);
    float interval = pathLen / (numPoints - 1);
    std::vector<Point> resampled;
    resampled.push_back(points[0]);
    
    float accumulatedDist = 0;
    for (size_t i = 1; i < points.size() && resampled.size() < numPoints; i++) {
        float dx = points[i].x - points[i-1].x;
        float dy = points[i].y - points[i-1].y;
        float segmentLen = hypot(dx, dy);
        
        if (accumulatedDist + segmentLen >= interval) {
            float t = (interval - accumulatedDist) / segmentLen;
            Point newPoint(
                points[i-1].x + t * dx,
                points[i-1].y + t * dy
            );
            resampled.push_back(newPoint);
            points.insert(points.begin() + i, newPoint);
            accumulatedDist = 0;
        } else {
            accumulatedDist += segmentLen;
        }
    }
    
    while (resampled.size() < numPoints) {
        resampled.push_back(points.back());
    }
    
    points = resampled;
}

Point GestureAnalyzer::centeroid(const std::vector<Point>& points) const {
    Point center(0, 0);
    for (const auto& p : points) {
        center.x += p.x;
        center.y += p.y;
    }
    center.x /= points.size();
    center.y /= points.size();
    return center;
}

void GestureAnalyzer::scaleToUnit(std::vector<Point>& points) const {
    float minX = INFINITY, minY = INFINITY;
    float maxX = -INFINITY, maxY = -INFINITY;
    
    for (const auto& p : points) {
        minX = std::min(minX, static_cast<float>(p.x));
        minY = std::min(minY, static_cast<float>(p.y));
        maxX = std::max(maxX, static_cast<float>(p.x));
        maxY = std::max(maxY, static_cast<float>(p.y));
    }
    
    float scale = std::max(maxX - minX, maxY - minY);
    if (scale > 0) {
        for (auto& p : points) {
            p.x = (p.x - minX) / scale;
            p.y = (p.y - minY) / scale;
        }
    }
}

float GestureAnalyzer::pathLength(const std::vector<Point>& points) const {
    float length = 0;
    for (size_t i = 1; i < points.size(); i++) {
        length += hypot(points[i].x - points[i-1].x, points[i].y - points[i-1].y);
    }
    return length;
}