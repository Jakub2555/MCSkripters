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
        FILLED_RECT,
        CIRCLE,
        FILLED_CIRCLE,
        LINE,
        TRIANGLE,
        FILLED_TRIANGLE
    };

    struct DrawCommand_t
    {
        EDrawType type;
        std::string text;
        float x, y, x2, y2, x3, y3;
        float radius;
        float thickness;
        uint32_t color;
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
    void AddRect(int id, float x, float y, float w, float h, uint32_t color);
    void AddFilledRect(int id, float x, float y, float w, float h, uint32_t color);
    void AddText(int id, std::string text, float x, float y, float size, uint32_t color);
    void AddCircle(int id, float x, float y, float radius, float thickness, uint32_t color);
    void AddFilledCircle(int id, float x, float y, float radius, uint32_t color);
    void AddLine(int id, float x, float y, float x2, float y2, float thickness, uint32_t color);
    void AddTriangle(int id, float x, float y, float x2, float y2, float x3, float y3, float thickness, uint32_t color);
    void AddFilledTriangle(int id, float x, float y, float x2, float y2, float x3, float y3, uint32_t color);
}