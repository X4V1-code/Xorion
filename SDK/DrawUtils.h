#pragma once
#include "MinecraftUIRenderContext.h"
#include "MatrixStack.h"
#include "Tessellator.h"
#include "MC_Color.h"
#include "AABB.h"
#include "Vec2.h"
#include "Vec3.h"

// Minimal, engine-agnostic draw helpers.
// NOTE: Integrate with your actual UI/text/tessellator API where marked.
namespace DrawUtils {
    // Call once per frame/render pass to provide contexts
    void init(MinecraftUIRenderContext* ctx, MatrixStack* ms, Tessellator* t);

    // 2D text (HUD/overlay)
    void drawText(const Vec2& pos, const char* text, const MC_Color& color, float scale = 1.0f);

    // 3D primitives
    void drawLine3D(const Vec3& a, const Vec3& b, const MC_Color& color, float lineWidth = 1.0f);
    void drawBox3D(const AABB& box, const MC_Color& color, float lineWidth = 1.0f, float alpha = 0.5f);

    // Convenience grid crosshair at world position
    void drawCross3D(const Vec3& center, float size, const MC_Color& color, float lineWidth = 1.0f);
}
