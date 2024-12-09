#include "ImageProcessor.h"
#include <cstring>
#include <cmath>

namespace {
    using Constants::Display::SCREEN_WIDTH;
    using Constants::Display::SCREEN_HEIGHT;
}

bool ImageProcessor::begin() {
    setupDefaultFilters();
    return true;
}

void ImageProcessor::setupDefaultFilters() {
    // Filtre gaussien pour le flou
    float gaussianKernel[3][3] = {
        {1.0f/16, 2.0f/16, 1.0f/16},
        {2.0f/16, 4.0f/16, 2.0f/16},
        {1.0f/16, 2.0f/16, 1.0f/16}
    };
    addCustomFilter("gaussian", gaussianKernel, 1.0f, 0.0f);

    // Filtre de Sobel pour la détection des bords
    float sobelXKernel[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    addCustomFilter("sobel_x", sobelXKernel, 1.0f, 128.0f);

    float sobelYKernel[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };
    addCustomFilter("sobel_y", sobelYKernel, 1.0f, 128.0f);

    // Filtre de netteté
    float sharpenKernel[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };
    addCustomFilter("sharpen", sharpenKernel);
}

void ImageProcessor::addCustomFilter(const String& name, const float kernel[3][3], float factor, float bias) {
    // Créer un filtre avec le kernel donné
    ImageFilter filter;
    filter.name = name;
    memcpy(filter.kernel, kernel, 9 * sizeof(float));
    filter.factor = factor;
    filter.bias = bias;
    filters[name] = std::move(filter);
}

void ImageProcessor::removeFilter(const String& name) {
    filters.erase(name);
}

void ImageProcessor::applyFilter(const String& filterName) {
    auto it = filters.find(filterName);
    if (it != filters.end()) {
        applyKernel(it->second.kernel, it->second.factor, it->second.bias);
    }
}

void ImageProcessor::applyKernel(const float kernel[3][3], float factor, float bias) {
    if (processedImage.empty()) return;

    std::vector<uint8_t> temp = processedImage;
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;

    for (int y = 1; y < height-1; y++) {
        for (int x = 1; x < width-1; x++) {
            float sum = 0;
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int px = x + kx;
                    int py = y + ky;
                    sum += kernel[ky+1][kx+1] * temp[py * width + px];
                }
            }
            int result = (int)(sum * factor + bias);
            processedImage[y * width + x] = constrain(result, 0, 255);
        }
    }
}

void ImageProcessor::processImage(SensorData& data) {
    // Créer une image en niveaux de gris à partir des données
    processedImage.resize(SCREEN_WIDTH * SCREEN_HEIGHT);
    
    // Initialiser avec des pixels blancs
    std::fill(processedImage.begin(), processedImage.end(), 255);
    
    // Dessiner les points détectés en noir
    for (const auto& point : data.points) {
        if (point.x >= 0 && point.x < SCREEN_WIDTH &&
            point.y >= 0 && point.y < SCREEN_HEIGHT) {
            processedImage[point.y * SCREEN_WIDTH + point.x] = 0;
        }
    }
    
    if (denoiseLevel > 0) {
        denoise();
    }
    
    if (motionStabilization) {
        stabilizeMotion();
    }
}

void ImageProcessor::setDenoiseLevel(int level) {
    denoiseLevel = constrain(level, 0, 10);
}

void ImageProcessor::applyMotionStabilization(bool enable) {
    motionStabilization = enable;
}

std::vector<String> ImageProcessor::getAvailableFilters() const {
    std::vector<String> filterNames;
    for (const auto& filter : filters) {
        filterNames.push_back(filter.first);
    }
    return filterNames;
}

void ImageProcessor::denoise() {
    if (processedImage.empty()) return;

    for (int i = 0; i < denoiseLevel; i++) {
        applyFilter("gaussian");
    }
}

void ImageProcessor::stabilizeMotion() {
    // Détection des coins avec Harris
    auto corners = detectHarrisCorners();
    if (corners.empty()) return;

    // Calculer la transformation rigide optimale
    // Pour simplifier, on utilise juste la moyenne des déplacements
    int dx = 0, dy = 0;
    for (const auto& corner : corners) {
        dx += corner.x;
        dy += corner.y;
    }
    dx /= corners.size();
    dy /= corners.size();

    // Appliquer la correction
    for (auto& point : corners) {
        point.x -= dx;
        point.y -= dy;
    }
}

std::vector<Point> ImageProcessor::detectHarrisCorners() const {
    using Constants::Display::SCREEN_WIDTH;
    using Constants::Display::SCREEN_HEIGHT;
    
    std::vector<Point> corners;
    if (processedImage.empty()) return corners;

    // Paramètres de Harris
    const float k = 0.04f;
    const float threshold = 10000.0f;

    // Calcul des dérivées
    for (int y = 1; y < Constants::SCREEN_HEIGHT-1; y++) {
        for (int x = 1; x < Constants::SCREEN_WIDTH-1; x++) {
            float Ix = processedImage[y * Constants::SCREEN_WIDTH + x+1] - 
                      processedImage[y * Constants::SCREEN_WIDTH + x-1];
            float Iy = processedImage[(y+1) * Constants::SCREEN_WIDTH + x] - 
                      processedImage[(y-1) * Constants::SCREEN_WIDTH + x];

            float Ix2 = Ix * Ix;
            float Iy2 = Iy * Iy;
            float Ixy = Ix * Iy;

            // Matrice de Harris
            float det = (Ix2 * Iy2) - (Ixy * Ixy);
            float trace = Ix2 + Iy2;
            float response = det - k * (trace * trace);

            if (response > threshold) {
                corners.push_back(Point(x, y));
            }
        }
    }

    return corners;
}

std::vector<std::vector<Point>> ImageProcessor::detectContours() const {
    std::vector<std::vector<Point>> contours;
    if (processedImage.empty()) return contours;

    // Détection des bords
    auto edges = detectEdges();
    if (edges.empty()) return contours;

    // Suivi des contours
    std::vector<bool> visited(edges.size(), false);
    for (size_t i = 0; i < edges.size(); i++) {
        if (!visited[i]) {
            std::vector<Point> contour;
            Point current = edges[i];
            visited[i] = true;
            contour.push_back(current);

            bool foundNext = true;
            while (foundNext) {
                foundNext = false;
                for (size_t j = 0; j < edges.size(); j++) {
                    if (!visited[j]) {
                        const Point& neighbor = edges[j];
                        int dx = abs(neighbor.x - current.x);
                        int dy = abs(neighbor.y - current.y);
                        if (dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0)) {
                            visited[j] = true;
                            contour.push_back(neighbor);
                            current = neighbor;
                            foundNext = true;
                            break;
                        }
                    }
                }
            }

            if (contour.size() > 10) { // Filtrer les petits contours
                contours.push_back(contour);
            }
        }
    }

    return contours;
}

std::vector<Point> ImageProcessor::detectEdges() const {
    using Constants::Display::SCREEN_WIDTH;
    using Constants::Display::SCREEN_HEIGHT;
    
    std::vector<Point> edges;
    if (processedImage.empty()) return edges;

    // Calcul du gradient avec Sobel
    for (int y = 1; y < Constants::SCREEN_HEIGHT-1; y++) {
        for (int x = 1; x < Constants::SCREEN_WIDTH-1; x++) {
            float gx = 0, gy = 0;
            
            // Application du filtre de Sobel
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    int px = x + kx;
                    int py = y + ky;
                    float pixel = processedImage[py * Constants::SCREEN_WIDTH + px];
                    
                    gx += pixel * filters.at("sobel_x").kernel[ky+1][kx+1];
                    gy += pixel * filters.at("sobel_y").kernel[ky+1][kx+1];
                }
            }
            
            float magnitude = sqrt(gx*gx + gy*gy);
            if (magnitude > 128) { // Seuil arbitraire
                edges.push_back(Point(x, y));
            }
        }
    }

    return edges;
}

float ImageProcessor::calculateBlurriness() const {
    using Constants::Display::SCREEN_WIDTH;
    using Constants::Display::SCREEN_HEIGHT;
    
    if (processedImage.empty()) return 0.0f;

    float totalVariance = 0.0f;
    int count = 0;

    for (int y = 1; y < Constants::SCREEN_HEIGHT-1; y++) {
        for (int x = 1; x < Constants::SCREEN_WIDTH-1; x++) {
            // Calcul de Laplacien
            float center = processedImage[y * Constants::SCREEN_WIDTH + x];
            float up = processedImage[(y-1) * Constants::SCREEN_WIDTH + x];
            float down = processedImage[(y+1) * Constants::SCREEN_WIDTH + x];
            float left = processedImage[y * Constants::SCREEN_WIDTH + x-1];
            float right = processedImage[y * Constants::SCREEN_WIDTH + x+1];

            float laplacian = abs(4*center - up - down - left - right);
            totalVariance += laplacian;
            count++;
        }
    }

    return count > 0 ? totalVariance / count : 0.0f;
}