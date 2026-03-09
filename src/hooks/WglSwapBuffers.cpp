#include "Hooks.h"

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui.h>

#include <render/Menu.h>
#include <render/Renderer.h>
#include <InputSystem.h>

HGLRC oContext;
HGLRC newContext;
bool init = false;

BOOL __stdcall hooks::HkWglSwapBuffers(HDC hdc)
{
    if (!init)
    {
        oContext = wglGetCurrentContext();
        newContext = wglCreateContext(hdc);

        wglMakeCurrent(hdc, newContext);

        render::Initialize();
        inputSystem::Initialize();

        wglMakeCurrent(hdc, oContext);

        init = true;
        return oSwapBuffers(hdc);
    }

    render::Swap();

    wglMakeCurrent(hdc, newContext);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    menu::Render();
    render::Render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    wglMakeCurrent(hdc, oContext);

    return oSwapBuffers(hdc);
}