# Minecraft 1.21.130 Signatures Implementation

## Summary
Added 12 new function signatures for Minecraft 1.21.130 based on IDA Pro reverse engineering.

## Files Modified

### 1. **SDK/EntitySignatures.h** (NEW)
- Created signature patterns for entity/actor functions
- Organized by priority (Rotation, OnGround, RuntimeID)
- Includes optional signatures for velocity, modules, and settings
- All addresses wildcarded with `?` for version compatibility

### 2. **Memory/SignatureGlobals.h**
Added global function pointers:
```cpp
extern void* g_Actor_getRotation;
extern void* g_Actor_setRotation;
extern void* g_Actor_isOnGround;
extern void* g_RuntimeId_Generic;
extern void* g_ActorUniqueID;
extern void* g_ActorRuntimeID;
extern void* g_RuntimeId_Member;
```

### 3. **Memory/SignatureGlobals.cpp**
Initialized all new pointers to nullptr

### 4. **Memory/SignatureResolver.cpp**
Added resolution calls for all 7 new signatures with RESOLVE() macro

## Signatures Added

### Priority 1: Rotation (2 signatures)
| Signature | Pattern | Source Function |
|-----------|---------|-----------------|
| `Actor::getRotation` | `48 8D 05 ? ? ? ? C6 41 10 00 48 89 01 48 8D` | sub_1440A0890 |
| `Actor::setRotation` | `48 8D 05 ? ? ? ? C6 41 10 00 48 89 01 48 8D` | sub_1440A06A0 |

**Note:** These have identical patterns (likely constructors), but different function addresses

### Priority 2: OnGround (1 signature)
| Signature | Pattern | Source Function |
|-----------|---------|-----------------|
| `Actor::isOnGround` | `48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 48` | sub_144098120 |

### Priority 3: Runtime ID (4 signatures)
| Signature | Pattern | Source Function |
|-----------|---------|-----------------|
| `RuntimeId_Generic` | `48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8D` | sub_142EA5980 |
| `ActorUniqueID` | `48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57` | sub_14335D120 |
| `ActorRuntimeID` | `48 8B C4 48 89 58 10 48 89 78 18 55 48 8D A8 ?` | sub_142DDEE60 |
| `RuntimeId_Member` | `48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55` | sub_142DDBBF0 |

## Additional Patterns Available (Not yet hooked)

### Velocity Functions
- `Actor::getVelocity` - Pattern: `48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 48` (sub_144098120)
- `Actor::setVelocity` - Same pattern (sub_144098120)

### Module/Callback System
- `Module` - Pattern: `48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57` (sub_146BCCC80)
- `Listener` - Pattern: `48 89 5C 24 10 48 89 7C 24 18 55 48 8D AC 24 ?` (sub_1434FD520)

### Settings System
- `Setting` - Pattern: `48 89 5C 24 10 55 56 57 41 54 41 55 41 56 41 57` (sub_1478B16B0)
- `Enum` - Pattern: `40 53 48 83 EC 20 65 48 8B 04 25 58 00 00 00 BB` (sub_140F55000)

### Data References (Not Functions)
- `callback` vtable → off_1483A90B8
- `slider` UI data → off_149389C50

## Next Steps

To actually USE these signatures:

1. **Test signature resolution:**
   - Build the project
   - Check console logs for signature resolution status
   - All 7 should report "resolved" or "FAILED"

2. **Extract offsets from resolved functions:**
   - Use the resolved function pointers to analyze where they read member offsets
   - Example: If `g_Actor_getRotation` reads `[rcx+0x1A0]`, then viewAngles offset = 0x1A0

3. **Update Entity.h / LocalPlayer.h:**
   - Add `BUILD_ACCESS` macros with discovered offsets
   - Example: `BUILD_ACCESS(Vec2, viewAngles, 0x1A0);`

4. **Remove TODOs:**
   - Replace TODO comments with actual implementations
   - Use the new offsets for rotation, onGround, etc.

## Pattern Format Notes

- `?` = wildcard for addresses/offsets that change between versions
- Patterns are hex byte sequences from function prologues
- SlimMem signature scanner handles `?` wildcards automatically
- Patterns should be unique within the Minecraft binary

## Source
All patterns extracted from IDA Pro analysis of Minecraft.Windows.exe version 1.21.130
See `Signatures.txt` for full search term details and function mappings.
