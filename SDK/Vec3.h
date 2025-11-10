#pragma once
#include <cmath>
#include <limits>

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vec3() = default;
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Arithmetic
    inline Vec3 operator+(const Vec3& o) const { return {x + o.x, y + o.y, z + o.z}; }
    inline Vec3 operator-(const Vec3& o) const { return {x - o.x, y - o.y, z - o.z}; }
    inline Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
    inline Vec3 operator/(float s) const { return s != 0.0f ? Vec3{x / s, y / s, z / s} : Vec3{x, y, z}; }

    inline Vec3& operator+=(const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
    inline Vec3& operator-=(const Vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    inline Vec3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
    inline Vec3& operator/=(float s) { if (s != 0.0f) { x /= s; y /= s; z /= s; } return *this; }

    // Norms
    inline float length() const { return std::sqrt(x * x + y * y + z * z); }
    inline float lengthSquared() const { return x * x + y * y + z * z; }
    inline Vec3 normalized() const {
        float len = length();
        return len > std::numeric_limits<float>::epsilon() ? Vec3{x / len, y / len, z / len} : Vec3{0.0f, 0.0f, 0.0f};
    }

    // Dot & cross
    inline static float dot(const Vec3& a, const Vec3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    inline static Vec3 cross(const Vec3& a, const Vec3& b) {
        return Vec3{
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    // Forward vector from yaw/pitch in degrees (right-handed, Y up)
    static Vec3 fromYawPitch(float yawDeg, float pitchDeg);
};

// Implementation kept here to avoid circular includes
inline Vec3 Vec3::fromYawPitch(float yawDeg, float pitchDeg) {
    constexpr float PI = 3.14159265358979323846f;
    float yaw = yawDeg * PI / 180.0f;
    float pitch = pitchDeg * PI / 180.0f;
    float cp = std::cos(pitch);
    return Vec3{
        -std::sin(yaw) * cp,   // X
        -std::sin(pitch),      // Y
         std::cos(yaw) * cp    // Z
    };
}
