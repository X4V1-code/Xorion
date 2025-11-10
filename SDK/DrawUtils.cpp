#include "DrawUtils.h"
#include <cstring>

// Global render state (provided via init)
static MinecraftUIRenderContext* gCtx = nullptr;
static MatrixStack* gMS = nullptr;
static Tessellator* gTess = nullptr;

void DrawUtils::init(MinecraftUIRenderContext* ctx, MatrixStack* ms, Tessellator* t) {
    gCtx = ctx;
    gMS = ms;
    gTess = t;
}

void DrawUtils::drawText(const Vec2& pos, const char* text, const MC_Color& color, float scale) {
    (void)scale;
    if (!gCtx || !text) return;

    // Placeholder: Replace with your actual UI font rendering API.
    // Example (pseudocode):
    // gCtx->drawText(*gMS, pos.x, pos.y, TextHolder(text), color.r, color.g, color.b, color.a, scale);
    (void)pos;
    (void)color;
}

void DrawUtils::drawLine3D(const Vec3& a, const Vec3& b, const MC_Color& color, float lineWidth) {
    if (!gTess || !gMS) return;
    (void)lineWidth;

    // Placeholder tessellation logic. Replace with your engine's line primitive.
    // Example (pseudocode):
    // gTess->begin(LineList, 1.0f);
    // gTess->color(color.r, color.g, color.b, color.a);
    // gTess->vertex(a.x, a.y, a.z);
    // gTess->vertex(b.x, b.y, b.z);
    // gTess->end(*gCtx, *gMS);
    (void)a; (void)b; (void)color;
}

void DrawUtils::drawBox3D(const AABB& box, const MC_Color& color, float lineWidth, float alpha) {
    if (!gTess || !gMS) return;
    (void)lineWidth; (void)alpha;

    // Draw 12 edges of the box. Replace with your engine draw calls.
    Vec3 v000 = box.min;
    Vec3 v111 = box.max;
    Vec3 v001{box.min.x, box.min.y, box.max.z};
    Vec3 v010{box.min.x, box.max.y, box.min.z};
    Vec3 v011{box.min.x, box.max.y, box.max.z};
    Vec3 v100{box.max.x, box.min.y, box.min.z};
    Vec3 v101{box.max.x, box.min.y, box.max.z};
    Vec3 v110{box.max.x, box.max.y, box.min.z};

    // Placeholder: invoke drawLine3D for each edge.
    drawLine3D(v000, v100, color, lineWidth);
    drawLine3D(v000, v010, color, lineWidth);
    drawLine3D(v000, v001, color, lineWidth);

    drawLine3D(v111, v101, color, lineWidth);
    drawLine3D(v111, v110, color, lineWidth);
    drawLine3D(v111, v011, color, lineWidth);

    drawLine3D(v100, v101, color, lineWidth);
    drawLine3D(v100, v110, color, lineWidth);

    drawLine3D(v010, v011, color, lineWidth);
    drawLine3D(v010, v110, color, lineWidth);

    drawLine3D(v001, v011, color, lineWidth);
    drawLine3D(v001, v101, color, lineWidth);
}

void DrawUtils::drawCross3D(const Vec3& c, float size, const MC_Color& color, float lineWidth) {
    Vec3 dx{size, 0.0f, 0.0f};
    Vec3 dy{0.0f, size, 0.0f};
    Vec3 dz{0.0f, 0.0f, size};
    drawLine3D(c - dx, c + dx, color, lineWidth);
    drawLine3D(c - dy, c + dy, color, lineWidth);
    drawLine3D(c - dz, c + dz, color, lineWidth);
}
