cmake_minimum_required(VERSION 3.0)
project(app)
set(AUTHOR "Mohammed Alyousef")
set(PROJ_VERSION 0.1.0)

set(CMAKE_CXX_FLAGS -DIMGUI_IMPL_OPENGL_LOADER_GL3W)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenGL_GL_PREFERENCE GLVND)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set some Win32 Specific Settings
IF(WIN32)
    SET(GUI_TYPE WIN32)
ENDIF(WIN32)

# Set some Apple MacOS Specific settings
IF (APPLE)
    SET(GUI_TYPE MACOSX_BUNDLE)
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

add_subdirectory(sdl2)

find_package(OpenGL REQUIRED)

set(IMGUI_SRCS imgui/imgui.cpp
    imgui/imgui_demo.cpp 
    imgui/imgui_draw.cpp 
    imgui/imgui_widgets.cpp
    imgui/examples/imgui_impl_sdl.cpp
    imgui/examples/imgui_impl_opengl2.cpp 
    imgui/examples/libs/gl3w/GL/gl3w.c
    )

set(PROJ_SRCS src/main.cpp)

add_executable(${PROJECT_NAME} ${GUI_TYPE} ${PROJ_SRCS} ${IMGUI_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR} 
    inc
    sdl2/include
    imgui
    imgui/examples 
    imgui/examples/libs/gl3w
    )

target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${OPENGL_opengl_LIBRARY} 
    ${OPENGL_glx_LIBRARY}
    SDL2main
    SDL2-static
    ${CMAKE_DL_LIBS}
    )
