cmake_minimum_required(VERSION 3.15)
project(mcskripters)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -m64)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -m64)

set(MAIN_SRC
    src/Main.cpp
)

set(MC_SRC
    src/mc/Java.cpp
)

set(HOOKS_SRC
    src/hooks/Hooks.cpp
    src/hooks/WglSwapBuffers.cpp
)

set(UTILS_SRC
    src/utils/InputSystem.cpp
    src/utils/render/Menu.cpp
    src/utils/render/Renderer.cpp
)

set(LUA_SRC
    src/lua/Script.cpp
)

set(THIRDPARTY_SRC 
    # MinHook
    src/thirdparty/minhook/buffer.c
    src/thirdparty/minhook/hook.c
    src/thirdparty/minhook/trampoline.c
    src/thirdparty/minhook/hde/hde32.c
    src/thirdparty/minhook/hde/hde64.c

    # ImGui
    src/thirdparty/imgui/imgui.cpp
    src/thirdparty/imgui/imgui_draw.cpp
    src/thirdparty/imgui/imgui_impl_opengl3.cpp
    src/thirdparty/imgui/imgui_impl_win32.cpp
    src/thirdparty/imgui/imgui_tables.cpp
    src/thirdparty/imgui/imgui_widgets.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${THIRDPARTY_SRC}
    ${MC_SRC}
    ${HOOKS_SRC}
    ${UTILS_SRC}
    ${LUA_SRC}
    ${MAIN_SRC}
)

target_compile_options(${PROJECT_NAME} PRIVATE -mavx -mavx2)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN)

target_link_libraries(${PROJECT_NAME} PRIVATE
    opengl32.lib
    ${CMAKE_SOURCE_DIR}/src/thirdparty/jni/lib/jvm.lib
    ${CMAKE_SOURCE_DIR}/src/thirdparty/luajit/lib/lua51.lib
    ${CMAKE_SOURCE_DIR}/src/thirdparty/luajit/lib/luajit.lib
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/thirdparty
    ${CMAKE_SOURCE_DIR}/src/mc
    ${CMAKE_SOURCE_DIR}/src/hooks
    ${CMAKE_SOURCE_DIR}/src/utils
    ${CMAKE_SOURCE_DIR}/src/lua
)