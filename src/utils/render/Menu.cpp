#include "Menu.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <Script.h>

void menu::LuaTab()
{
    ImGui::SetCursorPosY(70);

    ImGui::BeginListBox("##scripts", { 200, 390 });
    {
        for (int i = 0; i < scriptsManager->scripts.size(); ++i)
        {
            if (ImGui::Selectable(scriptsManager->scripts[i]->name.c_str()))
            {
                currentLua = i;
                ImGui::OpenPopup("script options");
            }
        }

        if (ImGui::BeginPopup(("script options")))
        {
            if (ImGui::Button("Load", { 100, 25 }))
            {
                scriptsManager->LoadScript(currentLua);
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::Button("Unload", { 100, 25 }))
            {
                scriptsManager->UnloadScript(currentLua);
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::EndListBox();
    }

    if (ImGui::Button("Refresh", { 200, 25 }))
        scriptsManager->RefreshScripts();
}

void menu::Render()
{
    if (!open)
        return;

    ImGui::SetNextWindowSize({ 570, 500 });
	ImGui::Begin("MCSkripters", &menu::open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
	{
        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize();

        ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + size, ImColor(0, 20, 38, 210));

        ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + ImVec2(size.x, 60), ImColor(0, 14, 28, 210));

        ImGui::SetCursorPosY(15);
        ImGui::SetCursorPosX(size.x - 555);
        ImGui::Tab("MISC", { 100, 30 }, 4, &currentTab);

        ImGui::SetCursorPosY(15);
        ImGui::SetCursorPosX(size.x - 445);
        ImGui::Tab("MOVEMENT", { 100, 30 }, 3, &currentTab);

        ImGui::SetCursorPosY(15);
        ImGui::SetCursorPosX(size.x - 335);
        ImGui::Tab("VISUALS", { 100, 30 }, 2, &currentTab);

        ImGui::SetCursorPosY(15);
        ImGui::SetCursorPosX(size.x - 225);
        ImGui::Tab("COMBAT", { 100, 30 }, 1, &currentTab);

        ImGui::SetCursorPosY(15);
        ImGui::SetCursorPosX(size.x - 115);
        ImGui::Tab("LUA", { 100, 30 }, 0, &currentTab);

        switch (currentTab)
        {
        case 0:
            LuaTab();
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        }

        ImGui::End();
	}
}