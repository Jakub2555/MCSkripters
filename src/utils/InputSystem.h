#pragma once

#include <Windows.h>
#include <cstdint>

namespace inputSystem
{
    inline WNDPROC oWndProc;

    bool Initialize();
    void Shutdown();

    bool KeyHandler(UINT msg, WPARAM wParam, LPARAM lParam);

    LONG_PTR __stdcall WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
}