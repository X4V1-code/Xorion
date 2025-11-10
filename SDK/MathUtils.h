#pragma once
#include <cmath>
#include <algorithm>

namespace MathUtils {
    constexpr float PI = 3.14159265358979323846f;
    constexpr float TWO_PI = 2.0f * PI;

    inline float deg2rad(float deg) { return deg * PI / 180.0f; }
    inline float rad2deg(float rad) { return rad * 180.0f / PI; }

    inline float clamp(float v, float lo, float hi) { return std::max(lo, std::min(v, hi)); }

    inline float wrapDegrees(float deg) {
        float r = std::fmod(deg, 360.0f);
        if (r < -180.0f) r += 360.0f;
        if (r > 180.0f) r -= 360.0f;
        return r;
    }
}
