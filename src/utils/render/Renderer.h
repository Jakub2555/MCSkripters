#pragma once

#include <Windows.h>
#include <string>
#include <mutex>
#include <vector>
#include <unordered_map>

#include <imgui/imgui.h>

namespace render
{
    enum EDrawType
    {
        TEXT = 0,
        RECT,
        FILLED_RECT
    };

    struct DrawCommand_t
    {
        EDrawType type;
        std::string text;
        float x, y, w, h;
        ImVec4 color;
    };

    struct DrawCommandList_t
    {
        std::vector<DrawCommand_t> cmds;
        std::mutex mtx;
    };

    inline HWND window = NULL;

    // Script id | cmds
    inline std::mutex pendingMtx;
    inline std::unordered_map<int, DrawCommandList_t> pendingCmds;

    inline std::mutex renderMtx;
    inline std::vector<DrawCommand_t> renderCmds;

    bool Initialize();
    void Shutdown();

    void Swap();
    void Render();

    // Render functions
    void AddRect(int id, float x, float y, float w, float h, float r, float g, float b, float a);
    void AddFilledRect(int id, float x, float y, float w, float h, float r, float g, float b, float a);
    void AddText(int id, std::string text, float x, float y, float r, float g, float b, float a);
}