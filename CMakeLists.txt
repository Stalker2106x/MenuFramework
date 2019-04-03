cmake_minimum_required(VERSION 3.9)

project(MenuFramework
        VERSION 1.0.1
        LANGUAGES CXX
        DESCRIPTION "Menu Framework")

#[[
find_library(LIBLUA NAMES liblua_static.lib libliblua_static.a
             HINTS thirdparty/luac/build
             thirdparty/luac/build/Debug
             thirdparty/luac/build/Release)
]]

file(GLOB MenuFrameworkSrc "src/*.cpp")
file(GLOB PugiXMLSrc "thirdparty/pugixml/src/*.cpp")
file(GLOB LuaSrc "thirdparty/lua/src/*.c")

add_library(MenuFramework STATIC ${MenuFrameworkSrc} ${PugiXMLSrc} ${LuaSrc})

set_target_properties(MenuFramework PROPERTIES PUBLIC_HEADER include/MenuFramework.hpp)

include_directories(include)
include_directories(renderers)
include_directories(thirdparty)
include_directories(thirdparty/json/include)
include_directories(thirdparty/pugixml/src)
include_directories(thirdparty/sol2)
include_directories(thirdparty/lua/src)
#include_directories(thirdparty/luac/build) #added for luaconf.h

target_link_libraries(MenuFramework)