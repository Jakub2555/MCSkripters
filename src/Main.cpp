#include <Windows.h>

#include <Java.h>
#include <Hooks.h>
#include <render/Renderer.h>
#include <InputSystem.h>
#include <Script.h>

void Main(HMODULE hModule)
{
    java::jvm = java::GetJavaVM();

    scriptsManager = new CScriptsManager();

    hooks::Initialize();

    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(1);
    }

    delete scriptsManager;

    hooks::Shutdown();
    render::Shutdown();
    inputSystem::Shutdown();

    java::jvm->DetachCurrentThread();
    
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Main), hModule, 0, 0));
    }
    return TRUE;
}