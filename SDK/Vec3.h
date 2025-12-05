#pragma once
#include "Utils/HMath.h"

// Alias to existing Vec3 type
using vec3_t = ::Vec3;

// Add binary operators for Vec3 that use existing methods
inline Vec3 operator+(const Vec3& a, const Vec3& b) {
    return a.add(b);
}

inline Vec3 operator-(const Vec3& a, const Vec3& b) {
    return a.sub(b);
}

inline Vec3 operator*(const Vec3& a, float f) {
    return a.mul(f);
}

inline Vec3 operator/(const Vec3& a, float f) {
    return a.div(f);
}
