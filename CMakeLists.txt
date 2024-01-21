cmake_minimum_required(VERSION 3.15)

include(FetchContent)
FetchContent_Declare(
    IMGUI
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(IMGUI)

project(calc)
set(AUTHOR "Mohammed Alyousef")
set(PROJ_VERSION 0.1.0)

IF (APPLE)
    set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJ_VERSION})
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2024" ${AUTHOR})
ENDIF (APPLE)

# set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SDL_PTHREADS_ENABLED_BY_DEFAULT ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE Release)
endif()

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Prevented in-tree build. Please create a build directory outside of the source directory and call cmake from there.")
endif()

if (NOT(ANDROID))
    find_package(OpenGL REQUIRED)
endif()
find_package(Threads REQUIRED)

set(PROJ_SRCS src/main.cpp)

set(IMGUI_SRCS 
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp 
    ${imgui_SOURCE_DIR}/imgui_draw.cpp 
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    )

set(INCS 
    ${CMAKE_CURRENT_LIST_DIR} 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${imgui_SOURCE_DIR}/examples
    )

set(LIBS
    ${CMAKE_DL_LIBS}
    Threads::Threads
    )

if (NOT(ANDROID))
    set(LIBS ${LIBS} 
        ${OPENGL_opengl_LIBRARY}
        ${OPENGL_glx_LIBRARY}
        )
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(BINARY_NAME index)
    set(IMGUI_SRCS ${IMGUI_SRCS} ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
    add_executable(${BINARY_NAME} ${PROJ_SRCS} ${IMGUI_SRCS})
    target_compile_definitions(${BINARY_NAME} PRIVATE EMSCRIPTEN)
    set_target_properties(${BINARY_NAME} PROPERTIES SUFFIX .html COMPILE_FLAGS "-sUSE_SDL=2" LINK_FLAGS "-sASYNCIFY -sUSE_SDL=2 -sNO_FILESYSTEM=0 -sDISABLE_EXCEPTION_CATCHING=1 -sNO_EXIT_RUNTIME=0 -sASSERTIONS=1 -sLLD_REPORT_UNDEFINED -Wl,--no-check-features --shell-file ${CMAKE_CURRENT_LIST_DIR}/shell_minimal.html --no-heap-copy --preload-file ${CMAKE_CURRENT_LIST_DIR}/fonts@/fonts")
else()
    set(BINARY_NAME ${PROJECT_NAME})
    FetchContent_Declare(
        SDL2
        URL https://github.com/spurious/SDL-mirror/archive/master.zip
    )
    FetchContent_MakeAvailable(SDL2)
    set(LIBS ${LIBS} SDL2-static SDL2main)
    if (ANDROID)
        set(IMGUI_SRCS ${IMGUI_SRCS} ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
        add_library(${BINARY_NAME} SHARED ${PROJ_SRCS} ${IMGUI_SRCS})
        target_compile_definitions(-${BINARY_NAME} PRIVATE IMGUI_IMPL_OPENGL_ES2)
    else()
        set(IMGUI_SRCS ${IMGUI_SRCS} ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp ${imgui_SOURCE_DIR}/examples/libs/gl3w/GL/gl3w.c)
        set(LIBS ${LIBS} ${OPENGL_gl_LIBRARY})
        add_executable(${BINARY_NAME} WIN32 MACOSX_BUNDLE ${PROJ_SRCS} ${IMGUI_SRCS})
        target_compile_definitions(${BINARY_NAME} PRIVATE PRIVATE IMGUI_IMPL_OPENGL_LOADER_GL3W)
    endif()
endif()

target_compile_features(${BINARY_NAME} PRIVATE cxx_std_17)

target_include_directories(${BINARY_NAME} PRIVATE ${INCS})

target_link_libraries(${BINARY_NAME} PRIVATE ${LIBS})
