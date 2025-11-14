// DllMain.cpp
#include <Windows.h>
#include "Utils/Logger.h"
#include "InitSDK.h"
#include "GameDataChecks.h"

DWORD WINAPI InitThread(LPVOID) {
    if (!InitSDK()) {
        logF("[DllMain] InitSDK failed for 1.21.121. Modules will be guarded.");
    } else {
        logF("[DllMain] InitSDK OK for 1.21.121.");
    }
    VerifyGameData();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(nullptr, 0, InitThread, nullptr, 0, nullptr);
        if (hThread) CloseHandle(hThread);
    }
    return TRUE;
}
