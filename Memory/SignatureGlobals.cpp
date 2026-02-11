// Memory/SignatureGlobals.cpp
#include "SignatureGlobals.h"

void* g_WrapperEarlyCall      = nullptr;
void* g_WrapperCallB          = nullptr;
void* g_TwoArgCallA           = nullptr;
void* g_MethodRCX_RDX         = nullptr;
void* g_FieldReadCall         = nullptr;
void* g_GSReadA               = nullptr;
void* g_GSReadB               = nullptr;
void* g_GSReadC               = nullptr;
void* g_RipStaticA            = nullptr;
void* g_RipStaticB            = nullptr;
void* g_ConstCheckA           = nullptr;
void* g_ConstCheckB           = nullptr;
void* g_SSESetup              = nullptr;
void* g_GSReadD               = nullptr;
void* g_WrapperEarlyCallAlt   = nullptr;

// Entity/Actor function pointers
void* g_Actor_getRotation     = nullptr;
void* g_Actor_setRotation     = nullptr;
void* g_Actor_isOnGround      = nullptr;
void* g_RuntimeId_Generic     = nullptr;
void* g_ActorUniqueID         = nullptr;
void* g_ActorRuntimeID        = nullptr;
void* g_RuntimeId_Member      = nullptr;
