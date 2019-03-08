PROJECT ( lua )

IF( NOT WIN32 )
    message( FATAL_ERROR "Written for window only" )
ENDIF()

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# remove warnings
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS ) 

SET (HDR_LIBLUA
lapi.h lauxlib.h lcode.h lctype.h ldebug.h ldo.h lfunc.h
lgc.h llex.h llimits.h lmem.h lobject.h lopcodes.h lparser.h
lstate.h lstring.h ltable.h ltm.h lua.h luaconf.h lualib.h
lundump.h lvm.h lzio.h
)

#SET_SOURCE_FILES_PROPERTIES(${HDR_LIBLUA} PROPERTIES HEADER_FILE_ONLY TRUE)
#include_directories(src)

# Build Libraries
SET (SRC_LIBLUA
lapi.c lauxlib.c lbaselib.c lbitlib.c lcode.c lcorolib.c
lctype.c ldblib.c ldebug.c ldo.c ldump.c lfunc.c lgc.c
linit.c liolib.c llex.c lmathlib.c lmem.c loadlib.c lobject.c
lopcodes.c loslib.c lparser.c lstate.c lstring.c lstrlib.c
ltable.c ltablib.c ltm.c lundump.c lvm.c lzio.c lutf8lib.c
)

SET ( SRC_LUA lua.c )
SET ( SRC_LUAC luac.c )

# compile with C++ compiler
set_source_files_properties(${SRC_LIBLUA} ${SRC_LUA} ${SRC_LUAC} PROPERTIES LANGUAGE CXX)

# append headers to sources to make them show up in MSVC GUI
LIST(APPEND SRC_LIBLUA ${HDR_LIBLUA})

#Library
ADD_LIBRARY ( liblua ${SRC_LIBLUA} )

#DLL
ADD_LIBRARY ( lua5.2 SHARED ${SRC_LIBLUA} )
SET_TARGET_PROPERTIES (lua5.2 PROPERTIES DEFINE_SYMBOL  "LUA_BUILD_AS_DLL" )

#exe
ADD_EXECUTABLE ( lua    ${SRC_LUA}  )
ADD_EXECUTABLE ( luac  ${SRC_LUAC} )

#lua uses a DLL; luac uses a library
TARGET_LINK_LIBRARIES ( lua lua5.2 )
TARGET_LINK_LIBRARIES ( luac liblua )