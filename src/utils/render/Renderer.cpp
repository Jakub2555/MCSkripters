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
		switch (cmd.type)
		{
		case EDrawType::TEXT:
			drawList->AddText(ImGui::GetFont(), cmd.radius, { cmd.x, cmd.y }, cmd.color, cmd.text.c_str());
			break;
		case EDrawType::RECT:
			drawList->AddRect({ cmd.x, cmd.y }, { cmd.x2, cmd.y2 }, cmd.color);
			break;
		case EDrawType::FILLED_RECT:
			drawList->AddRectFilled({ cmd.x, cmd.y }, { cmd.x2, cmd.y2 }, cmd.color);
			break;
		case EDrawType::CIRCLE:
			drawList->AddCircle({ cmd.x, cmd.y }, cmd.radius, cmd.color, 0, cmd.thickness);
			break;
		case EDrawType::FILLED_CIRCLE:
			drawList->AddCircleFilled({ cmd.x, cmd.y }, cmd.radius, cmd.color);
			break;
		case EDrawType::LINE:
			drawList->AddLine({ cmd.x, cmd.y }, { cmd.x2, cmd.y2 }, cmd.color, cmd.thickness);
			break;
		case EDrawType::TRIANGLE:
			drawList->AddTriangle({ cmd.x, cmd.y }, { cmd.x2, cmd.y2 }, { cmd.x3, cmd.y3 }, cmd.color, cmd.thickness);
			break;
		case EDrawType::FILLED_TRIANGLE:
			drawList->AddTriangleFilled({ cmd.x, cmd.y }, { cmd.x2, cmd.y2 }, { cmd.x3, cmd.y3 }, cmd.color);
			break;
		}
	}
}

#pragma region render_funcs

void render::AddRect(int id, float x, float y, float w, float h, uint32_t color)
{
	DrawCommand_t cmd;
	cmd.type = EDrawType::RECT;

	cmd.x = x;
	cmd.y = y;
	cmd.x2 = w;
	cmd.y2 = h;
	cmd.color = color;

	std::lock_guard<std::mutex> l(render::pendingMtx);
	DrawCommandList_t& list = pendingCmds[id];

	std::lock_guard<std::mutex> l2(list.mtx);
	list.cmds.push_back(cmd);
}

void render::AddFilledRect(int id, float x, float y, float w, float h, uint32_t color)
{
	DrawCommand_t cmd;
	cmd.type = EDrawType::FILLED_RECT;

	cmd.x = x;
	cmd.y = y;
	cmd.x2 = w;
	cmd.y2 = h;
	cmd.color = color;

	std::lock_guard<std::mutex> l(render::pendingMtx);
	DrawCommandList_t& list = pendingCmds[id];

	std::lock_guard<std::mutex> l2(list.mtx);
	list.cmds.push_back(cmd);
}

void render::AddText(int id, std::string text, float x, float y, float size, uint32_t color)
{
	DrawCommand_t cmd;
	cmd.type = EDrawType::TEXT;

	cmd.x = x;
	cmd.y = y;
	cmd.text = text;
	cmd.radius = size;
	cmd.color = color;

	std::lock_guard<std::mutex> l(render::pendingMtx);
	DrawCommandList_t& list = pendingCmds[id];

	std::lock_guard<std::mutex> l2(list.mtx);
	list.cmds.push_back(cmd);
}

void render::AddCircle(int id, float x, float y, float radius, float thickness, uint32_t color)
{
	DrawCommand_t cmd;
	cmd.type = EDrawType::CIRCLE;

	cmd.x = x;
	cmd.y = y;
	cmd.radius = radius;
	cmd.thickness = thickness;
	cmd.color = color;

	std::lock_guard<std::mutex> l(render::pendingMtx);
	DrawCommandList_t& list = pendingCmds[id];

	std::lock_guard<std::mutex> l2(list.mtx);
	list.cmds.push_back(cmd);
}

void render::AddFilledCircle(int id, float x, float y, float radius, uint32_t color)
{
	DrawCommand_t cmd;
	cmd.type = EDrawType::FILLED_CIRCLE;

	cmd.x = x;
	cmd.y = y;
	cmd.radius = radius;
	cmd.color = color;

	std::lock_guard<std::mutex> l(render::pendingMtx);
	DrawCommandList_t& list = pendingCmds[id];

	std::lock_guard<std::mutex> l2(list.mtx);
	list.cmds.push_back(cmd);
}

void render::AddLine(int id, float x, float y, float x2, float y2, float thickness, uint32_t color)
{
	DrawCommand_t cmd;
	cmd.type = EDrawType::LINE;

	cmd.x = x;
	cmd.y = y;
	cmd.x2 = x2;
	cmd.y2 = y2;
	cmd.thickness = thickness;
	cmd.color = color;

	std::lock_guard<std::mutex> l(render::pendingMtx);
	DrawCommandList_t& list = pendingCmds[id];

	std::lock_guard<std::mutex> l2(list.mtx);
	list.cmds.push_back(cmd);
}

void render::AddTriangle(int id, float x, float y, float x2, float y2, float x3, float y3, float thickness, uint32_t color)
{
	DrawCommand_t cmd;
	cmd.type = EDrawType::TRIANGLE;

	cmd.x = x;
	cmd.y = y;
	cmd.x2 = x2;
	cmd.y2 = y2;
	cmd.x3 = x3;
	cmd.y3 = y3;
	cmd.thickness = thickness;
	cmd.color = color;

	std::lock_guard<std::mutex> l(render::pendingMtx);
	DrawCommandList_t& list = pendingCmds[id];

	std::lock_guard<std::mutex> l2(list.mtx);
	list.cmds.push_back(cmd);
}

void render::AddFilledTriangle(int id, float x, float y, float x2, float y2, float x3, float y3, uint32_t color)
{
	DrawCommand_t cmd;
	cmd.type = EDrawType::FILLED_TRIANGLE;

	cmd.x = x;
	cmd.y = y;
	cmd.x2 = x2;
	cmd.y2 = y2;
	cmd.x3 = x3;
	cmd.y3 = y3;
	cmd.color = color;

	std::lock_guard<std::mutex> l(render::pendingMtx);
	DrawCommandList_t& list = pendingCmds[id];

	std::lock_guard<std::mutex> l2(list.mtx);
	list.cmds.push_back(cmd);
}

#pragma endregion