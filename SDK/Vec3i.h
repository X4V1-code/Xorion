#pragma once

struct Vec3i {
    int x = 0;
    int y = 0;
    int z = 0;

    Vec3i() = default;
    Vec3i(int x, int y, int z) : x(x), y(y), z(z) {}

    inline Vec3i operator+(const Vec3i& o) const { return {x + o.x, y + o.y, z + o.z}; }
    inline Vec3i operator-(const Vec3i& o) const { return {x - o.x, y - o.y, z - o.z}; }
    inline bool operator==(const Vec3i& o) const { return x == o.x && y == o.y && z == o.z; }
    inline bool operator!=(const Vec3i& o) const { return !(*this == o); }
};
