#include "Renderer.h"

#include <Windows.h>
#include <gl/GL.h>

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui.h>

#include <Script.h>

bool render::Initialize()
{
    GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);

	window = WindowFromDC(wglGetCurrentDC());
    
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplWin32_InitForOpenGL(window);
	ImGui_ImplOpenGL3_Init();

	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);

    return true;
}

void render::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void render::Swap()
{
	std::vector<DrawCommand_t> merge;
	
	std::lock_guard<std::mutex> l1(pendingMtx);
	for (auto& [id, list] : pendingCmds)
	{
		std::lock_guard<std::mutex> l2(list.mtx);
		merge.insert(merge.end(), list.cmds.begin(), list.cmds.end());
	}

	std::lock_guard<std::mutex> l3(renderMtx);
	renderCmds.swap(merge);
}

void render::Render()
{
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();

	for (DrawCommand_t& cmd : renderCmds)
	{
		ImU32 color = ImGui::ColorConvertFloat4ToU32(cmd.color);

		switch (cmd.type)
		{
		case EDrawType::TEXT:
			drawList->AddText({ cmd.x, cmd.y }, color, cmd.text.c_str());
			break;
		case EDrawType::RECT:
			drawList->AddRect({ cmd.x, cmd.y }, { cmd.w, cmd.h }, color);
			break;
		case EDrawType::FILLED_RECT:
			drawList->AddRectFilled({ cmd.x, cmd.y }, { cmd.w, cmd.h }, color);
			break;
		}
	}
}

void render::AddRect(int id, float x, float y, float w, float h, float r, float g, float b, float a)
{
	DrawCommand_t cmd;
	cmd.type = EDrawType::RECT;

	cmd.x = x;
	cmd.y = y;
	cmd.w = w;
	cmd.h = h;
	cmd.color = { r, g, b, a };

	std::lock_guard<std::mutex> l(render::pendingMtx);
	DrawCommandList_t& list = pendingCmds[id];

	std::lock_guard<std::mutex> l2(list.mtx);
	list.cmds.push_back(cmd);
}

void render::AddFilledRect(int id, float x, float y, float w, float h, float r, float g, float b, float a)
{
	DrawCommand_t cmd;
	cmd.type = EDrawType::FILLED_RECT;

	cmd.x = x;
	cmd.y = y;
	cmd.w = w;
	cmd.h = h;
	cmd.color = { r, g, b, a };

	std::lock_guard<std::mutex> l(render::pendingMtx);
	DrawCommandList_t& list = pendingCmds[id];

	std::lock_guard<std::mutex> l2(list.mtx);
	list.cmds.push_back(cmd);
}

void render::AddText(int id, std::string text, float x, float y, float r, float g, float b, float a)
{
	DrawCommand_t cmd;
	cmd.type = EDrawType::TEXT;

	cmd.x = x;
	cmd.y = y;
	cmd.text = text;
	cmd.color = { r, g, b, a };

	std::lock_guard<std::mutex> l(render::pendingMtx);
	DrawCommandList_t& list = pendingCmds[id];

	std::lock_guard<std::mutex> l2(list.mtx);
	list.cmds.push_back(cmd);
}