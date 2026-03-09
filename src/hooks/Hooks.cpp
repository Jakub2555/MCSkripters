#include "Hooks.h"

#include <minhook/MinHook.h>

bool hooks::Initialize()
{
    if (MH_Initialize() != MH_OK)
        return false;

    if (MH_CreateHookApi(L"opengl32.dll", "wglSwapBuffers", &HkWglSwapBuffers, (void**)&oSwapBuffers) != MH_OK)
        return false;
    
    MH_EnableHook(MH_ALL_HOOKS);

    return true;
}

void hooks::Shutdown()
{
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
}