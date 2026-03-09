#pragma once

#include <windows.h>

namespace hooks
{
    bool Initialize();
    void Shutdown();

    typedef BOOL(__stdcall* wglSwapBuffers)(HDC);
	inline wglSwapBuffers oSwapBuffers;
	BOOL __stdcall HkWglSwapBuffers(HDC hdc);
}