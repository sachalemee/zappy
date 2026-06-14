##
## EPITECH PROJECT, 2026
## zappy
## File description:
## CMakeLists
##

cmake_minimum_required(VERSION 3.16)
project(zappy VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

if(MSVC)
    set(PROJECT_WARNING_FLAGS /W4 /WX)
else()
    set(PROJECT_WARNING_FLAGS -Wall -Wextra -Werror)
endif()

find_package(Threads REQUIRED)

find_package(raylib QUIET)

if(NOT raylib_FOUND)
    include(FetchContent)
    set(FETCHCONTENT_QUIET FALSE)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
    )
    FetchContent_MakeAvailable(raylib)
endif()

# SERVER

file(GLOB_RECURSE SERVER_SOURCES src/server/*.c)
list(REMOVE_ITEM SERVER_SOURCES ${CMAKE_SOURCE_DIR}/src/server/network/commands.c)

add_executable(zappy_server ${SERVER_SOURCES})

target_compile_options(zappy_server PRIVATE ${PROJECT_WARNING_FLAGS})

target_include_directories(zappy_server PRIVATE
    include
    include/server
)

target_link_libraries(zappy_server PRIVATE Threads::Threads)

# GUI

file(GLOB_RECURSE GUI_SOURCES src/gui/*.cpp)

add_executable(zappy_gui ${GUI_SOURCES})

target_compile_options(zappy_gui PRIVATE ${PROJECT_WARNING_FLAGS})

target_include_directories(zappy_gui PRIVATE
    include
    include/gui
)

target_link_libraries(zappy_gui PRIVATE raylib Threads::Threads m dl rt)

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(zappy_gui PRIVATE X11)
endif()

# AI

add_custom_target(zappy_ai ALL
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/src/ai/zappy_ai.py
            ${CMAKE_SOURCE_DIR}/zappy_ai
    COMMENT "Copying zappy_ai"
)
