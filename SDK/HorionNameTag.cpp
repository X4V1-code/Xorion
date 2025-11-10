#include "HorionNameTag.h"
#include "FontRepository.h"   // or whatever your client uses to manage fonts

void HorionNameTag::renderLocalPlayerName() {
    C_LocalPlayer* lp = g_Data.getLocalPlayer();
    if (lp == nullptr) return;

    // Position above the player's head
    vec3_t pos = *lp->getPos();
    pos.y += lp->height + 0.5f;

    // Grab the player's username
    std::string name = lp->getNameTag()->getText();

    // Use the Horion font (replace with your actual font object)
    C_Font* horionFont = g_Data.getFontRepository()->getFont("Horion"); 
    if (horionFont == nullptr) {
        // fallback to default if Horion font not loaded
        horionFont = g_Data.getFontRepository()->getFont("Default");
    }

    // Draw the text
    DrawUtils::drawText(pos, &name, MC_Color(255, 255, 255), 1.0f, horionFont);
}
