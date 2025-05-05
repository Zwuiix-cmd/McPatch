include(FetchContent)
cmake_minimum_required(VERSION 3.29)
project(McPatch)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s -DNDEBUG -Os")
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(BUILD_TESTING OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fno-unwind-tables -fno-asynchronous-unwind-tables")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-threadsafe-statics")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--strip-all")

add_compile_definitions(_ITERATOR_DEBUG_LEVEL=0)

add_executable(McPatch WIN32 main.cpp
        src/utils/Game.h
        src/utils/Logger.h
        src/utils/Logger.h
        src/SDK/SDK.hpp
        src/utils/json.hpp
        src/utils/Internet.h
        src/Form/FormMenu.hpp)


FetchContent_Declare(
        rang
        GIT_REPOSITORY https://github.com/agauniyal/rang.git
        GIT_TAG master
)
FetchContent_GetProperties(rang)
if(NOT rang_POPULATED)
    FetchContent_Populate(rang)
    add_subdirectory(${rang_SOURCE_DIR} ${rang_BINARY_DIR})
endif()

message("-- Downloaded Rang")

target_link_libraries(McPatch PRIVATE rang)
target_link_libraries(McPatch PRIVATE "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64/wininet.lib")
target_link_libraries(McPatch PRIVATE Psapi)
