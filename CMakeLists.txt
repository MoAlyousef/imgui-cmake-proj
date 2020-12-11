cmake_minimum_required(VERSION 3.15)
project(app)
set(AUTHOR "Mohammed Alyousef")
set(PROJ_VERSION 0.1.0)

include(FetchContent)

FetchContent_Declare(
    IMGUI
    URL https://github.com/ocornut/imgui/archive/master.zip
)
FetchContent_MakeAvailable(IMGUI)

set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SDL_PTHREADS_ENABLED_BY_DEFAULT ON)

# Set some Apple MacOS Specific settings
IF (APPLE)
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJ_VERSION})
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2020" ${AUTHOR})
ENDIF (APPLE)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release)
endif()

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Prevented in-tree build. Please create a build directory outside of the source directory and call cmake from there.")
endif()

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

set(PROJ_SRCS src/main.cpp)

set(IMGUI_SRCS 
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp 
    ${imgui_SOURCE_DIR}/imgui_draw.cpp 
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
    )

set(INCS 
    ${CMAKE_CURRENT_LIST_DIR} 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${imgui_SOURCE_DIR}/examples
    ${imgui_SOURCE_DIR}/examples/libs/gl3w
    )

set(LIBS 
    ${OPENGL_opengl_LIBRARY} 
    ${OPENGL_glx_LIBRARY}
    ${CMAKE_DL_LIBS}
    Threads::Threads
    )

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-DEMSCRIPTEN -s USE_SDL=2 -s NO_FILESYSTEM=0")
    set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-s WASM=1 -s USE_PTHREADS=1 -s ALLOW_MEMORY_GROWTH=1 -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=0 -s ASSERTIONS=1 -s LLD_REPORT_UNDEFINED -Wl,--shared-memory,--no-check-features --shared-memory --no-check-features --shell-file shell_minimal.html --no-heap-copy --preload-file ${CMAKE_CURRENT_LIST_DIR}/fonts@/fonts")
    set(IMGUI_SRCS ${IMGUI_SRCS} ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
else()
    FetchContent_Declare(
        SDL2
        URL https://github.com/spurious/SDL-mirror/archive/master.zip
    )
    FetchContent_MakeAvailable(SDL2)
    set(CMAKE_CXX_FLAGS -DIMGUI_IMPL_OPENGL_LOADER_GL3W)
    set(IMGUI_SRCS ${IMGUI_SRCS} ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp ${imgui_SOURCE_DIR}/examples/libs/gl3w/GL/gl3w.c)
    set(INCS ${INCS} sdl2/include)
    set(LIBS ${LIBS} SDL2-static SDL2main)
endif()

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${PROJ_SRCS} ${IMGUI_SRCS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX .html)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${INCS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS})
