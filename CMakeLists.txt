cmake_minimum_required(VERSION 3.2)

project(MenuFramework
        VERSION 1.0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project
file(GLOB MenuFrameworkInclude
    "include/*.hpp"
    "include/*.hh")
file(GLOB MenuFrameworkSrc
    "src/*.cpp")
file(GLOB MenuFrameworkRenderers
    "renderers/*/*.hpp"
    "renderers/*/*.cpp")
source_group("Renderers" FILES ${MenuFrameworkRenderers})

# PugiXML Dependency
file(GLOB PugiXMLSrc "thirdparty/pugixml/src/*.cpp")
source_group("PugiXML" FILES ${PugiXMLSrc})

# LUA Dependency
file(GLOB LuaSrc "thirdparty/lua/src/*.c")
source_group("LUA" FILES ${LuaSrc})

add_library(MenuFramework STATIC ${MenuFrameworkInclude} ${MenuFrameworkSrc} ${MenuFrameworkRenderers} ${PugiXMLSrc} ${LuaSrc})

set_source_files_properties(${LuaSrc} PROPERTIES LANGUAGE CXX)

set_target_properties(MenuFramework PROPERTIES
  PUBLIC_HEADER include/MenuFramework.hpp
  ARCHIVE_OUTPUT_DIRECTORY lib
  LIBRARY_OUTPUT_DIRECTORY lib)

include_directories(include)
include_directories(renderers)
include_directories(thirdparty)
include_directories(thirdparty/json/include)
include_directories(thirdparty/pugixml/src)
include_directories(thirdparty/lua/src)
include_directories(thirdparty/sol2)

target_link_libraries(MenuFramework)
