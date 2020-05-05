cmake_minimum_required(VERSION 3.0)
project(imgui)

set(CMAKE_CXX_FLAGS -DIMGUI_IMPL_OPENGL_LOADER_GL3W)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
set(SRCS main.cpp 
    examples/imgui_impl_sdl.cpp
    examples/imgui_impl_opengl2.cpp 
    imgui.cpp 
    imgui_demo.cpp 
    imgui_draw.cpp 
    imgui_widgets.cpp
    examples/libs/gl3w/GL/gl3w.c
    )
add_library(imgui SHARED ${SRCS})
target_include_directories(imgui PUBLIC 
    ${SDL2_INCLUDE_DIRS} 
    ${CMAKE_CURRENT_LIST_DIR} 
    examples 
    examples/libs/gl3w
    )
target_link_libraries(imgui PUBLIC 
    ${OPENGL_gl_LIBRARY} 
    ${SDL2_LIBRARIES} 
    ${CMAKE_DL_LIBS}
    )