cmake_minimum_required(VERSION 3.9)

project(MenuFrameworkTest
        VERSION 1.0.1
        LANGUAGES CXX
        DESCRIPTION "Menu Framework Test")

find_library(MenuFramework NAMES MenuFramework.lib MenuFramework.a
             HINTS ../build/Debug/ ../build/Release/ ../lib/ )

find_library(LIBLUA NAMES liblua_static.lib libliblua_static.a
             HINTS ../thirdparty/luac/build
                   ../thirdparty/luac/build/Debug
                   ../thirdparty/luac/build/Release)

include_directories(../include)
include_directories(../thirdparty/json/include/)
include_directories(../thirdparty/pugixml/src/)
include_directories(../thirdparty/sol2/)

add_executable(MenuFrameworkTest basic.cpp)
target_link_libraries(MenuFrameworkTest ${MenuFramework} ${LIBLUA})