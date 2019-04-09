cmake_minimum_required(VERSION 3.2)

project(MenuFramework
        VERSION 1.0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)


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


# Library CXX/Linker

add_library(MenuFramework STATIC ${MenuFrameworkInclude} ${MenuFrameworkSrc} ${MenuFrameworkRenderers} ${PugiXMLSrc} ${LuaSrc})

set_target_properties(${PROJECT_NAME} PROPERTIES
PUBLIC_HEADER include/MenuFramework.hpp
ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set_source_files_properties(${LuaSrc} PROPERTIES LANGUAGE CXX)

# Unit Tests

file(GLOB TestSrc "test/*.cpp")


# Test CXX/Linker

add_executable(${PROJECT_NAME}_test ${TestSrc})

target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}_test PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Continuous integration

if (CMAKE_BUILD_TYPE STREQUAL "Coverage" AND CMAKE_COMPILER_IS_GNUCXX)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    SETUP_TARGET_FOR_COVERAGE_LCOV(
        NAME ${PROJECT_NAME}_coverage
        EXECUTABLE ${PROJECT_NAME}_test
        DEPENDENCIES ${PROJECT_NAME}_test)
endif()

# Include

include_directories(include)
include_directories(renderers)
include_directories(thirdparty)
include_directories(thirdparty/json/include)
include_directories(thirdparty/pugixml/src)
include_directories(thirdparty/lua/src)
include_directories(thirdparty/sol2)

target_link_libraries(${PROJECT_NAME})
