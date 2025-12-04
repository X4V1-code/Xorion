#pragma once

#include <cstddef>

#ifdef _WIN32
#include <windows.h>
#endif

// FindSignature is a macro in Utils.h that calls Utils::FindSignatureModule
// Pattern format: "48 89 5C 24 ? 57 48 83 EC ?" where '?' or "??" are wildcards.
// Returns uintptr_t


