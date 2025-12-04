#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "../Utils/Colors.h"   // color constants for modules that draw or log
#include "../Utils/Utils.h"    // common helpers used by some modules (CallVFunc etc.)
#include "../../../SDK/GameMode.h"  // For C_GameMode
#include "../../../SDK/MoveInputHandler.h"  // For onMove hook
#include "../../../SDK/MinecraftUIRenderContext.h"  // For render hooks

// Forward declarations (use struct consistently across SDK)
struct Packet;
class Player; // Player is a class
struct GameMode;

// Small POD for UI/layout positioning
struct ModulePos {
    float x = 0.0f;
    float y = 0.0f;
};

// Keybind type (fits Windows VK codes)
using KeyBind = uint32_t;

// Module categories (adjust as needed)
enum class Category : uint8_t {
    COMBAT = 0,
    VISUAL,
    MOVEMENT,
    PLAYER,
    WORLD,
    MISC,
    CLIENT,
    CUSTOM
};

// Legacy unscoped enum aliases for backward compatibility
constexpr Category COMBAT = Category::COMBAT;
constexpr Category VISUAL = Category::VISUAL;
constexpr Category MOVEMENT = Category::MOVEMENT;
constexpr Category PLAYER = Category::PLAYER;
constexpr Category WORLD = Category::WORLD;
constexpr Category MISC = Category::MISC;
constexpr Category CLIENT = Category::CLIENT;
constexpr Category CUSTOM = Category::CUSTOM;

// Settings system stubs (EnumEntry and SettingEnum)
class EnumEntry {
private:
    std::string name;
    int value;
public:
    EnumEntry(const std::string& name, int value) : name(name), value(value) {}
    std::string GetName() const { return name; }
    int GetValue() const { return value; }
};

class IModule; // Forward declare

class SettingEnum {
public:
    std::vector<EnumEntry> Entrys;
    IModule* module;
    int selected = 0; // Currently selected index
    
    SettingEnum(IModule* mod) : module(mod) {}
    
    SettingEnum& addEntry(const EnumEntry& entry) {
        Entrys.push_back(entry);
        return *this;
    }
    
    int getSelectedValue() const {
        // Stub: return first entry value or 0
        return Entrys.empty() ? 0 : Entrys[0].GetValue();
    }
    
    EnumEntry GetSelectedEntry() const {
        // Stub: return first entry or default
        return Entrys.empty() ? EnumEntry("", 0) : Entrys[0];
    }
    
    EnumEntry& GetEntry(int index) {
        return Entrys[index];
    }
    
    int GetCount() const {
        return static_cast<int>(Entrys.size());
    }
};

// Base module interface used by all modules.
// Declares the hooks modules expect to override.
class IModule {
public:
    IModule(KeyBind keybind = 0, Category category = Category::MISC, const std::string& description = "")
        : keybind(keybind), category(category), description(description) {}
    virtual ~IModule() = default;

    // Identification
    virtual const char* getModuleName() { return "Unnamed"; }
    virtual const char* getRawModuleName() { return getModuleName(); }
    virtual const std::string& getDescription() const { return description; }

    // Auto-start behavior
    virtual bool allowAutoStart() { return true; }

    // Lifecycle
    virtual void onEnable() {}
    virtual void onDisable() {}

    // Core hooks (unified signatures)
    virtual void onTick(C_GameMode* gm) {}
    virtual void onPlayerTick(Player* player) {}
    virtual void onSendPacket(Packet* packet) {}

    // Render hooks (UI)
    virtual void onPreRender(MinecraftUIRenderContext* ctx) {}
    virtual void onPostRender(MinecraftUIRenderContext* ctx) {}
    virtual void onLevelRender() {}

    // Input / keybind handling
    virtual void onKeyUpdate(uint32_t key, bool isDown) {}
    virtual void onMove(MoveInputHandler* handler) {}

    // Toggle helpers
    bool isEnabled() const { return enabled; }
    void setEnabled(bool e) {
        if (enabled == e) return;
        enabled = e;
        if (enabled) onEnable(); else onDisable();
    }
    void toggle() { setEnabled(!enabled); }

    // Metadata
    Category getCategory() const { return category; }
    KeyBind getKeybind() const { return keybind; }
    void setKeybind(KeyBind kb) { keybind = kb; }

    // Position for HUD/arraylist layout
    ModulePos& getModulePos() { return pos; }
    void setModulePos(float x, float y) { pos.x = x; pos.y = y; }

    // Setting registration stubs
    void registerEnumSetting(const char* name, SettingEnum* setting, int defaultValue) { /* stub */ }
    void registerFloatSetting(const char* name, float* value, float defaultVal, float min, float max) { /* stub */ }
    void registerIntSetting(const char* name, int* value, int defaultVal, int min, int max) { /* stub */ }
    void registerBoolSetting(const char* name, bool* value, bool defaultVal) { /* stub */ }

    // Additional methods
    virtual std::string getTooltip() { return description; }
    virtual bool isFlashMode() { return false; }
    virtual bool callWhenDisabled();
    virtual void onLoadConfig(void* config) { /* stub */ }
    virtual void onSaveConfig(void* config) { /* stub */ }

protected:
    bool enabled = false;
    KeyBind keybind = 0;
    Category category = Category::MISC;
    std::string description;
    ModulePos pos{};
};
