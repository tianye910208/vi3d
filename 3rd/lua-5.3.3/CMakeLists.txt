cmake_minimum_required(VERSION 2.8)

if (WIN32 AND NOT CYGWIN)
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT" CACHE STRING "")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd" CACHE STRING "")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT" CACHE STRING "")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd" CACHE STRING "")
endif ()

project(lua)


include_directories(
    src
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set (LUA_CORE src/lapi.c src/lcode.c src/lctype.c src/ldebug.c src/ldo.c src/ldump.c
    src/lfunc.c src/lgc.c src/llex.c src/lmem.c src/lobject.c src/lopcodes.c src/lparser.c
    src/lstate.c src/lstring.c src/ltable.c src/ltm.c src/lundump.c src/lvm.c src/lzio.c)
set (LUA_LIB src/lauxlib.c src/lbaselib.c src/lbitlib.c src/lcorolib.c src/ldblib.c
    src/liolib.c src/lmathlib.c src/loslib.c src/lstrlib.c src/ltablib.c src/linit.c
    src/lutf8lib.c src/loadlib.c)



if (APPLE)
    if (IOS)
        set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
        add_library(lua STATIC
           ${LUA_CORE}
           ${LUA_LIB}
        )
    else ()
        set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD_32_64_BIT)")
        add_library(lua MODULE
            ${LUA_CORE}
            ${LUA_LIB}
        )
        set_target_properties ( lua PROPERTIES BUNDLE TRUE )
    endif ()
else ()
    add_library(lua STATIC
        ${LUA_CORE}
        ${LUA_LIB}
    )
endif ()

if (WIN32 AND NOT CYGWIN)
#    add_definitions(-DLUA_USE_WINDOWS)
else ()
    add_definitions(-DLUA_USE_POSIX)
    target_link_libraries(lua m)
endif ()

set_property(TARGET lua PROPERTY C_STANDARD 99)



