#pragma once

namespace menu
{
    inline bool open = false;
    inline int currentTab = 0;

    inline int currentLua = 0;

    void Render();

    // Tabs
    void LuaTab();
}