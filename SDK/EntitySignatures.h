// SDK/EntitySignatures.h
#pragma once

// Entity, Actor, and Player function signatures for Minecraft 1.21.130
// Extracted from IDA Pro analysis - see Signatures.txt for details

namespace EntitySignatures {

    // ======================
    // PRIORITY 1: ROTATION
    // ======================

    // Actor::getRotation - returns Vec2 rotation (yaw, pitch)
    inline constexpr const char* PATTERN_Actor_getRotation =
        // Function: sub_1440A0890
        // Signature has LEA offset at bytes 3-6, needs wildcards
        "48 8D 05 ? ? ? ? C6 41 10 00 48 89 01 48 8D";

    // Actor::setRotation - sets rotation from Vec2
    inline constexpr const char* PATTERN_Actor_setRotation =
        // Function: sub_1440A06A0
        // Signature has LEA offset at bytes 3-6, needs wildcards
        "48 8D 05 ? ? ? ? C6 41 10 00 48 89 01 48 8D";


    // ======================
    // PRIORITY 2: ONGROUND
    // ======================

    // Actor::isOnGround - returns bool if entity is on ground
    inline constexpr const char* PATTERN_Actor_isOnGround =
        // Function: sub_144098120
        "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 48";


    // ======================
    // PRIORITY 3: RUNTIME ID
    // ======================

    // Generic runtime ID accessor (might be constructor/helper)
    inline constexpr const char* PATTERN_RuntimeId_Generic =
        // Function: sub_142EA5980
        "48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8D";

    // ActorUniqueID related function
    inline constexpr const char* PATTERN_ActorUniqueID =
        // Function: sub_14335D120
        "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57";

    // ActorRuntimeID accessor
    inline constexpr const char* PATTERN_ActorRuntimeID =
        // Function: sub_142DDEE60
        // SUB RSP with large immediate at bytes 9-12, needs wildcards
        "48 8B C4 48 89 58 10 48 89 78 18 55 48 8D A8 ?";

    // Member variable RuntimeID accessor
    inline constexpr const char* PATTERN_RuntimeId_Member =
        // Function: sub_142DDBBF0
        "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55";


    // ======================
    // OPTIONAL: VELOCITY
    // ======================

    // Actor::getVelocity - returns Vec3 velocity
    inline constexpr const char* PATTERN_Actor_getVelocity =
        // Function: sub_144098120 (same as isOnGround - might be overloaded or in same class)
        "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 48";

    // Actor::setVelocity - sets velocity from Vec3
    inline constexpr const char* PATTERN_Actor_setVelocity =
        // Function: sub_144098120 (same as isOnGround)
        "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 48";


    // ======================
    // OPTIONAL: MODULE/CALLBACKS
    // ======================

    // Module system function
    inline constexpr const char* PATTERN_Module =
        // Function: sub_146BCCC80
        "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57";

    // Event listener registration
    inline constexpr const char* PATTERN_Listener =
        // Function: sub_1434FD520
        // Has LEA with large displacement at bytes 11-14, needs wildcards
        "48 89 5C 24 10 48 89 7C 24 18 55 48 8D AC 24 ?";


    // ======================
    // OPTIONAL: SETTINGS
    // ======================

    // Settings system function
    inline constexpr const char* PATTERN_Setting =
        // Function: sub_1478B16B0
        "48 89 5C 24 10 55 56 57 41 54 41 55 41 56 41 57";

    // Enum/enumeration handling
    inline constexpr const char* PATTERN_Enum =
        // Function: sub_140F55000
        "40 53 48 83 EC 20 65 48 8B 04 25 58 00 00 00 BB";


    // ======================
    // DATA REFERENCES (not functions)
    // ======================
    // Note: These are vtables or data pointers, not executable functions
    // Use these with caution - they point to read-only data sections
    
    // Callback vtable/data reference
    // Data: off_1483A90B8 (not a function - likely vtable or function pointer array)
    
    // Slider UI data reference  
    // Data: off_149389C50 (not a function - likely vtable or UI data)
}
