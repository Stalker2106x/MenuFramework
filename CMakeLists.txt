cmake_minimum_required(VERSION 3.2)

project(MenuFramework
        VERSION 1.0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
      
file(GLOB MenuFrameworkSrc "src/*.cpp")
file(GLOB PugiXMLSrc "thirdparty/pugixml/src/*.cpp")
file(GLOB LuaSrc "thirdparty/lua/src/*.c")

add_library(MenuFramework STATIC ${MenuFrameworkSrc} ${PugiXMLSrc} ${LuaSrc})

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
include_directories(thirdparty/sol2)
include_directories(thirdparty/lua/src)
#include_directories(thirdparty/luac/build) #added for luaconf.h

target_link_libraries(MenuFramework)
