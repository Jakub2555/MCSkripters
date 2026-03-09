#include "InputSystem.h"

#include <imgui/imgui_impl_win32.h>

#include <render/Menu.h>
#include <render/Renderer.h>
#include "render/Menu.h"

bool inputSystem::Initialize()
{
    oWndProc = (WNDPROC)SetWindowLongPtrA(render::window, GWLP_WNDPROC, (LONG_PTR)WndProc);
    if (!oWndProc)
        return false;

    return true;
}

void inputSystem::Shutdown()
{
    if (oWndProc != nullptr)
    {
        SetWindowLongPtrW(render::window, GWLP_WNDPROC, (LONG_PTR)oWndProc);
        oWndProc = nullptr;
    }
}

bool inputSystem::KeyHandler(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if (wParam == VK_INSERT)
        {
            const bool wasDown = (lParam & (1 << 30)) != 0;
            if (!wasDown)
                menu::open = !menu::open;
        }

        break;
    default:
        break;
    }

    return true;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall inputSystem::WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    KeyHandler(msg, wParam, lParam);

	if (menu::open)
	{
		ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam);
		return true;
	}

	return CallWindowProcA(oWndProc, window, msg, wParam, lParam);
}