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
            .addFunction("drawText", std::function<void(std::string, float, float, float, float, float, float)>([this](std::string text, float x, float y, float r, float g, float b, float a) {
                render::AddText(id, text, x, y, r, g, b, a);
            }))
            .addFunction("drawRect", std::function<void(float, float, float, float, float, float, float, float)>([this](float x, float y, float w, float h, float r, float g, float b, float a) {
                render::AddRect(id, x, y, w, h, r, g, b, a);
            }))
            .addFunction("drawFilledRect", std::function<void(float, float, float, float, float, float, float, float)>([this](float x, float y, float w, float h, float r, float g, float b, float a) {
                render::AddFilledRect(id, x, y, w, h, r, g, b, a);
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