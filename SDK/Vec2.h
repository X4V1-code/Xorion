#pragma once
#include <cmath>
#include <limits>

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}

    // Arithmetic
    inline Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    inline Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    inline Vec2 operator*(float s) const { return {x * s, y * s}; }
    inline Vec2 operator/(float s) const { return s != 0.0f ? Vec2{x / s, y / s} : Vec2{x, y}; }

    inline Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    inline Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
    inline Vec2& operator*=(float s) { x *= s; y *= s; return *this; }
    inline Vec2& operator/=(float s) { if (s != 0.0f) { x /= s; y /= s; } return *this; }

    // Norms
    inline float length() const { return std::sqrt(x * x + y * y); }
    inline float lengthSquared() const { return x * x + y * y; }
    inline Vec2 normalized() const {
        float len = length();
        return len > std::numeric_limits<float>::epsilon() ? Vec2{x / len, y / len} : Vec2{0.0f, 0.0f};
    }

    // Dot product
    inline static float dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }
};
