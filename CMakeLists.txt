cmake_minimum_required(VERSION 3.9)

project(MenuFramework VERSION 1.0.1 DESCRIPTION "Menu Framework")

find_library(LIBLUA NAMES liblua.lib liblua.a HINTS thirdparty/lua)

add_library(MenuFramework SHARED
    src/Localization.cpp
    src/GraphicsRenderer.cpp
    src/Menu.cpp
    src/MenuItem.cpp
    src/ScriptEngine.cpp
    src/DataFile.cpp
    thirdparty/pugixml/src/pugixml.cpp
)

set_target_properties(MenuFramework PROPERTIES PUBLIC_HEADER include/MenuFramework.hpp)

include_directories(include)
include_directories(thirdparty)
include_directories(thirdparty/json/include)
include_directories(thirdparty/pugixml/src)
include_directories(thirdparty/sol2)
include_directories(thirdparty/lua)

target_link_libraries(MenuFramework ${LIBLUA})