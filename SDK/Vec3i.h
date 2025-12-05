#pragma once

struct vec3_ti {
    int x, y, z;
    
    vec3_ti() : x(0), y(0), z(0) {}
    vec3_ti(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
    
    vec3_ti add(int dx, int dy, int dz) const {
        return vec3_ti(x + dx, y + dy, z + dz);
    }
};
