# MCSkripters

**MCSkripters** is an injectable **Lua script engine** for Minecraft, written in C++.  
I got interested in Lua scripting, and since I’m familiar with Minecraft, I decided to integrate it into the game.  
Currently, it mainly supports basic rendering features.

## TODO

- [x] Add more rendering functions
- [ ] Bind JNI functions
- [ ] Implement Java method hooking
- [ ] Add menu customization (sliders, checkboxes, etc.)
- [ ] Improve logging and error handling
- [ ] Bind Minecraft functionalities (e.g., `SetCoords`, `SetRotations`) to Lua
- [ ] Improve rendering (current implementation is basic)
- [ ] Refactor code for readability
- [ ] Make cross-platform support
- [ ] Documentation of the custom lua functions
- [ ] Fix the ghetto Sleeps while unloading scripts

## Features

- Draw text, rectangles, and filled rectangles in-game

## Examples

> Example scripts are located in **examples** folder in repository.

## How to Build

1. Install **CMake**, **Ninja**, and **Clang**
2. Run `BuildDebug.bat` or `BuildRelease.bat`
3. Use the compiled DLL from the build folder

## How to Use

1. Inject the compiled DLL into `javaw.exe` (Minecraft instance)
2. Open the menu with **INSERT**
3. Refresh the scripts list and load/unload scripts

> Scripts are located in `C:\MCSkripters` by default (this path can be changed in the code).

## Credits

- [LuaBridge](https://github.com/vinniefalco/LuaBridge)
- [LuaJIT](https://luajit.org/)
- [MinHook](https://github.com/TsudaKageyu/minhook)
- [ImGui](https://github.com/ocornut/imgui/tree/master)