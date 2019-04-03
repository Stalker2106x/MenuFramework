cmake_minimum_required(VERSION 3.2)

project(MenuFrameworkTest
        VERSION 1.0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
      
find_library(MenuFramework NAMES MenuFramework.lib libMenuFramework.a HINTS ../lib ../build/lib)

include_directories(../include)
include_directories(../thirdparty/json/include/)
include_directories(../thirdparty/pugixml/src/)

add_executable(MenuFrameworkTest basic.cpp)
target_link_libraries(MenuFrameworkTest ${MenuFramework})
