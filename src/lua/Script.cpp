#include "Script.h"

#include <Java.h>

#include <unordered_map>
#include <filesystem>
#include <algorithm>

using namespace luabridge;

void CScript::Run()
{
    lua = luaL_newstate();
    luaL_openlibs(lua);

    getGlobalNamespace(lua)
        .beginNamespace("jvm")
            .addFunction("GetENV", java::GetEnv)
        .endNamespace()
        .beginNamespace("render")
            .addFunction("drawText", std::function<void(std::string, float, float, float, uint32_t)>([this](std::string text, float size, float x, float y, uint32_t color) {
                render::AddText(id, text, x, y, size, color);
            }))
            .addFunction("drawRect", std::function<void(float, float, float, float, uint32_t)>([this](float x, float y, float w, float h, uint32_t color) {
                render::AddRect(id, x, y, w, h, color);
            }))
            .addFunction("drawFilledRect", std::function<void(float, float, float, float, uint32_t)>([this](float x, float y, float w, float h, uint32_t color) {
                render::AddFilledRect(id, x, y, w, h, color);
            }))
            .addFunction("drawFilledCircle", std::function<void(float, float, float, uint32_t)>([this](float x, float y, float radius, uint32_t color) {
                render::AddFilledCircle(id, x, y, radius, color);
            }))
            .addFunction("drawCircle", std::function<void(float, float, float, float, uint32_t)>([this](float x, float y, float radius, float thickness, uint32_t color) {
                render::AddCircle(id, x, y, radius, thickness, color);
            }))
            .addFunction("drawLine", std::function<void(float, float, float, float, float, uint32_t)>([this](float x, float y, float x2, float y2, float thickness, uint32_t color) {
                render::AddLine(id, x, y, x2, y2, thickness, color);
            }))
            .addFunction("drawTriangle", std::function<void(float, float, float, float, float, float, float, uint32_t)>([this](float x, float y, float x2, float y2, float x3, float y3, float thickness, uint32_t color) {
                render::AddTriangle(id, x, y, x2, y2, x3, y3, thickness, color);
            }))
            .addFunction("drawFilledTriangle", std::function<void(float, float, float, float, float, float, uint32_t)>([this](float x, float y, float x2, float y2, float x3, float y3, uint32_t color) {
                render::AddFilledTriangle(id, x, y, x2, y2, x3, y3, color);
            }))
        .endNamespace();

    std::string path = SCRIPTS_PATH + std::string("\\") + name;
    luaL_dofile(lua, path.c_str());

    running.store(true);

    // Mendatory functions
    LuaRef onInit = getGlobal(lua, "onInit");
    LuaRef onStop = getGlobal(lua, "onStop");

    // Optional ones
    LuaRef onUpdate = getGlobal(lua, "onUpdate");
    LuaRef onRender = getGlobal(lua, "onRender");

    if (!onInit.isFunction())
        MessageBoxA(NULL, "onInit not found!", "MCSkripters", MB_OK | MB_ICONERROR);
    if (!onStop.isFunction())
        MessageBoxA(NULL, "onStop not found!", "MCSkripters", MB_OK | MB_ICONERROR);

    // TODO: Handle missing different, currently just crashes.

    onInit();

    while (running.load())
    {
        if (onUpdate.isFunction())
            onUpdate();

        if (onRender.isFunction())
        {
            render::pendingMtx.lock();
            render::DrawCommandList_t& list = render::pendingCmds[id];

            list.mtx.lock();
            list.cmds.clear();
            list.mtx.unlock();

            render::pendingMtx.unlock();

            onRender();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    onStop();
}

void CScript::Stop()
{
    running.store(false);

    Sleep(1000);

    std::lock_guard<std::mutex> l(render::pendingMtx);
    render::pendingCmds.erase(id);
}

CScriptsManager::~CScriptsManager()
{
    for (int i = 0; i < scripts.size(); ++i)
        UnloadScript(i);
}

void CScriptsManager::RefreshScripts()
{
    std::lock_guard<std::mutex> lock(mtx);

    std::vector<std::string> foundFiles;

    for (auto& entry : std::filesystem::directory_iterator((SCRIPTS_PATH)))
    {
		if (entry.path().extension() == (".lua"))
			foundFiles.push_back(entry.path().filename().string());
	}

    scripts.erase(std::remove_if(scripts.begin(), scripts.end(), [&foundFiles](const auto& s) {
        if (std::find(foundFiles.begin(), foundFiles.end(), s->name) == foundFiles.end())
            return true;
        return false;
    }), scripts.end());

    for (std::string& name : foundFiles)
    {
        if (std::find_if(scripts.begin(), scripts.end(), [&](const auto& s) {
            return s->name == name;
        }) == scripts.end())
            scripts.push_back(std::make_unique<CScript>(name));
    }
}

bool CScriptsManager::LoadScript(int idx)
{
    std::lock_guard<std::mutex> lock(mtx);

    CScript* script = scripts[idx].get();
    script->id = idx;

    render::pendingMtx.lock();
    render::pendingCmds.try_emplace(idx);
    render::pendingMtx.unlock();

    if (script->running)
        return false;

    auto thread = std::thread([script]() {
        script->Run();
    });

    threads[idx] = std::move(thread);

    return true;
}

bool CScriptsManager::UnloadScript(int idx)
{
    std::lock_guard<std::mutex> lock(mtx);

    CScript* script = scripts[idx].get();

    if (!script->running)
        return false;

    script->Stop();

    Sleep(2000);
    
    auto t = threads.find(idx);
    if (t != threads.end())
    {
        if (t->second.joinable())
            t->second.join();

        threads.erase(t);
    }

    return true;
}