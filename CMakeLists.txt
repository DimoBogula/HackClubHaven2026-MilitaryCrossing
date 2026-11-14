cmake_minimum_required(VERSION 3.14)
project(MilitaryCrossing C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# simd stuff
include(FetchContent)

FetchContent_Declare(
    simdutf
    GIT_REPOSITORY https://github.com/simdutf/simdutf.git
    GIT_TAG        v9.3.2
)
FetchContent_MakeAvailable(simdutf)

FetchContent_Declare(
    simdjson
    GIT_REPOSITORY https://github.com/simdjson/simdjson.git
    GIT_TAG        v5.0.5
)
FetchContent_MakeAvailable(simdjson)

# source
file(GLOB_RECURSE C_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")
file(GLOB_RECURSE CXX_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

add_executable(MilitaryCrossing ${C_SRCS} ${CXX_SRCS})

target_include_directories(MilitaryCrossing PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    /usr/include/libpng16
)

if(MSVC)
    target_compile_options(MilitaryCrossing PRIVATE /W0 /Zi)
else()
    target_compile_options(MilitaryCrossing PRIVATE -w -g)
endif()

# ext libs
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_DIR "win64")
    else()
        set(ARCH_DIR "win32")
    endif()
    set(LIB_EXT "*.dll")
else()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(ARCH_DIR "linux64")
    else()
        set(ARCH_DIR "linux32")
    endif()
    set(LIB_EXT "*.so")
endif()

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/${ARCH_DIR}")
target_link_directories(MilitaryCrossing PRIVATE ${LIB_DIR})

file(GLOB LIB_FILES "${LIB_DIR}/${LIB_EXT}")
foreach(LIB_FILE ${LIB_FILES})
    get_filename_component(LIB_NAME ${LIB_FILE} NAME_WE)
    string(REGEX REPLACE "^lib" "" CLEAN_NAME ${LIB_NAME})
    target_link_libraries(MilitaryCrossing PRIVATE ${CLEAN_NAME})
endforeach()

# set output dir
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set_target_properties(MilitaryCrossing PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out"
    )
else()
    set_target_properties(MilitaryCrossing PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out"
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "$ORIGIN"
    )
endif()

# build
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_include_directories(MilitaryCrossing PRIVATE
        "/usr/x86_64-w64-mingw32/include/freetype2"
    )
    target_link_options(MilitaryCrossing PRIVATE
        -static-libgcc
        -static-libstdc++
    )
    target_link_libraries(MilitaryCrossing PRIVATE
        simdutf
        simdjson
        freetype
        glfw3
        opengl32
        gdi32
        winmm
        ole32
        shell32
    )
else()
    target_link_libraries(MilitaryCrossing PRIVATE
        simdutf
        simdjson
        freetype
        glfw
        GL
        X11
        pthread
        Xrandr
        Xi
        dl
    )
endif()
