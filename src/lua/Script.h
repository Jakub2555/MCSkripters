#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>

#include <luajit/lua.hpp>
#include <luabridge/LuaBridge.h>

#include <render/Renderer.h>

#define SCRIPTS_PATH "C:\\MCSkripters"

class CScript
{
private:
    lua_State* lua;

public:
    CScript(std::string n) : name(n) {};

    void Run();
    void Stop();

    std::atomic<bool> running = false;
    std::string name;
    int id;
};

class CScriptsManager
{
private:
    std::mutex mtx;
    std::unordered_map<int, std::thread> threads;

public:
    ~CScriptsManager();

    void RefreshScripts();
    bool LoadScript(int idx);
    bool UnloadScript(int idx);

    std::vector<std::unique_ptr<CScript>> scripts;
};

inline CScriptsManager* scriptsManager;