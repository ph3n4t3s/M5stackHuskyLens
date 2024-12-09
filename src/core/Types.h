#pragma once

struct Point {
    int x;
    int y;
    
    Point() : x(0), y(0) {}
    Point(int _x, int _y) : x(_x), y(_y) {}
};

struct Rect {
    int x, y, width, height;
    
    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(int _x, int _y, int w, int h) : x(_x), y(_y), width(w), height(h) {}
};

enum class SystemState {
    BOOTING,
    READY,
    ERROR
};