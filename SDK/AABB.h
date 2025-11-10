#pragma once
#include "Vec3.h"

struct AABB {
    Vec3 min;
    Vec3 max;

    AABB() = default;
    AABB(const Vec3& min, const Vec3& max) : min(min), max(max) {}

    inline bool contains(const Vec3& p) const {
        return p.x >= min.x && p.x <= max.x &&
               p.y >= min.y && p.y <= max.y &&
               p.z >= min.z && p.z <= max.z;
    }

    inline Vec3 center() const {
        return Vec3{(min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f};
    }

    inline Vec3 size() const {
        return Vec3{(max.x - min.x), (max.y - min.y), (max.z - min.z)};
    }
};
