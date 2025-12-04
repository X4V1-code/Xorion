#pragma once
#include <string>

struct InfoBoxData {
    std::string title;
    std::string message;
    bool isOpen = true;
    float fadeVal = 1.0f;
    float fadeTarget = 1.0f;
    float closeTimer = 0.0f;

    InfoBoxData(std::string t, std::string m)
        : title(std::move(t)), message(std::move(m)), isOpen(true), fadeVal(1.0f), fadeTarget(1.0f), closeTimer(0.0f) {}

    void fade() {
        // Dummy fade logic for compilation
        if (fadeVal > fadeTarget) fadeVal -= 0.05f;
        else if (fadeVal < fadeTarget) fadeVal += 0.05f;
        if (fadeVal < 0.0f) fadeVal = 0.0f;
        if (fadeVal > 1.0f) fadeVal = 1.0f;
    }
};
