#pragma once
#include <cstdint>

struct MC_Color {
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;

    MC_Color() = default;
    MC_Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}

    inline static MC_Color Red(uint8_t a = 255)   { return MC_Color{255, 0, 0, a}; }
    inline static MC_Color Green(uint8_t a = 255) { return MC_Color{0, 255, 0, a}; }
    inline static MC_Color Blue(uint8_t a = 255)  { return MC_Color{0, 0, 255, a}; }
    inline static MC_Color White(uint8_t a = 255) { return MC_Color{255, 255, 255, a}; }
    inline static MC_Color Black(uint8_t a = 255) { return MC_Color{0, 0, 0, a}; }
    inline static MC_Color Yellow(uint8_t a = 255){ return MC_Color{255, 255, 0, a}; }
    inline static MC_Color Cyan(uint8_t a = 255)  { return MC_Color{0, 255, 255, a}; }
    inline static MC_Color Magenta(uint8_t a = 255){ return MC_Color{255, 0, 255, a}; }
};
