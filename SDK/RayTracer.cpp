#include "RayTracer.h"
#include "MathUtils.h"

// Voxel DDA traversal (no world sampling): returns the first grid cell crossed along the ray.
static Vec3i floorVec3ToI(const Vec3& v) {
    return Vec3i{ static_cast<int>(std::floor(v.x)), static_cast<int>(std::floor(v.y)), static_cast<int>(std::floor(v.z)) };
}

HitResult RayTracer::voxelRay(const Vec3& origin, const Vec3& dirInput, float maxDist) {
    HitResult hr;
    hr.type = HitResultType::MISS;
    if (maxDist <= 0.0f) {
        hr.pos = origin;
        hr.blockPos = floorVec3ToI(origin);
        return hr;
    }

    // Normalize direction
    float len = std::sqrt(dirInput.x * dirInput.x + dirInput.y * dirInput.y + dirInput.z * dirInput.z);
    Vec3 dir = len > 0.0f ? Vec3{dirInput.x / len, dirInput.y / len, dirInput.z / len} : Vec3{0.0f, 0.0f, 0.0f};

    // Initial cell
    Vec3 pos = origin;
    Vec3i cell = floorVec3ToI(pos);

    // Step direction per axis
    int stepX = (dir.x > 0.0f) ? 1 : (dir.x < 0.0f ? -1 : 0);
    int stepY = (dir.y > 0.0f) ? 1 : (dir.y < 0.0f ? -1 : 0);
    int stepZ = (dir.z > 0.0f) ? 1 : (dir.z < 0.0f ? -1 : 0);

    // Compute tMax and tDelta for DDA
    auto nextBoundary = [](float p, int step) {
        if (step > 0) return std::floor(p) + 1.0f;
        if (step < 0) return std::floor(p);
        return std::numeric_limits<float>::infinity();
    };

    float tMaxX = (stepX != 0)
        ? (nextBoundary(pos.x, stepX) - pos.x) / dir.x
        : std::numeric_limits<float>::infinity();
    float tMaxY = (stepY != 0)
        ? (nextBoundary(pos.y, stepY) - pos.y) / dir.y
        : std::numeric_limits<float>::infinity();
    float tMaxZ = (stepZ != 0)
        ? (nextBoundary(pos.z, stepZ) - pos.z) / dir.z
        : std::numeric_limits<float>::infinity();

    float tDeltaX = (stepX != 0) ? static_cast<float>(stepX) / dir.x : std::numeric_limits<float>::infinity();
    float tDeltaY = (stepY != 0) ? static_cast<float>(stepY) / dir.y : std::numeric_limits<float>::infinity();
    float tDeltaZ = (stepZ != 0) ? static_cast<float>(stepZ) / dir.z : std::numeric_limits<float>::infinity();

    // Traverse up to maxDist
    float t = 0.0f;
    unsigned char lastFace = 255;

    while (t <= maxDist) {
        // Pick smallest tMax
        if (tMaxX <= tMaxY && tMaxX <= tMaxZ) {
            cell.x += stepX;
            t = tMaxX;
            tMaxX += tDeltaX;
            lastFace = (stepX > 0) ? 4 /*West*/ : 5 /*East*/;
        } else if (tMaxY <= tMaxZ) {
            cell.y += stepY;
            t = tMaxY;
            tMaxY += tDeltaY;
            lastFace = (stepY > 0) ? 0 /*Down*/ : 1 /*Up*/;
        } else {
            cell.z += stepZ;
            t = tMaxZ;
            tMaxZ += tDeltaZ;
            lastFace = (stepZ > 0) ? 2 /*North*/ : 3 /*South*/;
        }

        // Impact position along ray
        Vec3 impact{ origin.x + dir.x * t, origin.y + dir.y * t, origin.z + dir.z * t };

        // No engine sampling; we simply report the first crossed cell.
        hr.type = HitResultType::BLOCK;
        hr.pos = impact;
        hr.blockPos = cell;
        hr.face = lastFace;
        return hr;
    }

    // If we exited loop, it's a MISS
    hr.type = HitResultType::MISS;
    hr.pos = Vec3{ origin.x + dir.x * maxDist, origin.y + dir.y * maxDist, origin.z + dir.z * maxDist };
    hr.blockPos = floorVec3ToI(hr.pos);
    hr.face = 255;
    return hr;
}

HitResult RayTracer::voxelRayYawPitch(const Vec3& origin, float yawDeg, float pitchDeg, float maxDist) {
    // TODO: Vec3::fromYawPitch doesn't exist - need to implement direction calculation
    Vec3 dir(0, 0, 1);  // placeholder
    return voxelRay(origin, dir, maxDist);
}
