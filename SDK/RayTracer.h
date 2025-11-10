#pragma once
#include "Vec3.h"
#include "Vec3i.h"

// Minimal hit result scaffold to avoid engine dependencies
enum class HitResultType : unsigned char { MISS = 0, BLOCK = 1, ENTITY = 2 };

struct HitResult {
    HitResultType type = HitResultType::MISS;
    Vec3 pos{};       // Impact position (world space)
    Vec3i blockPos{}; // Block coordinates if a block was hit
    unsigned char face = 255; // 0..5 for faces, 255 for none
};

// Lightweight, client-side ray casting utilities (no engine internals)
namespace RayTracer {
    // Cast a ray from origin in direction (normalized internally) up to maxDist.
    // Performs a grid traversal (voxel DDA) to find the first block coordinate intersected.
    HitResult voxelRay(const Vec3& origin, const Vec3& direction, float maxDist);

    // Convenience: build direction from yaw/pitch (degrees) and cast
    HitResult voxelRayYawPitch(const Vec3& origin, float yawDeg, float pitchDeg, float maxDist);
}
