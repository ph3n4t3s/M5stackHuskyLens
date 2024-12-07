#pragma once

#include <algorithm>

// Define min/max functions for mixed integer types
template<typename T, typename U>
T max(T a, U b) {
    return (a > static_cast<T>(b)) ? a : static_cast<T>(b);
}

template<typename T, typename U>
T min(T a, U b) {
    return (a < static_cast<T>(b)) ? a : static_cast<T>(b);
}