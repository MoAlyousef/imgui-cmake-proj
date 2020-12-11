#include "imgui.h"
#include "imgui_impl_sdl.h"
#include <SDL.h>
#include <SDL_opengles2.h>
#include <stdio.h>
#include <stdlib.h>

#define FONT_PATH "fonts/FreeSans.ttf"

#ifdef EMSCRIPTEN
#include "imgui_impl_opengl3.h"
#include <emscripten.h>
#define OGL_INIT ImGui_ImplOpenGL3_Init
#define OGL_NEW_FRAME ImGui_ImplOpenGL3_NewFrame
#define OGL_RENDER_NEW_DATA ImGui_ImplOpenGL3_RenderDrawData
#else
#include "imgui_impl_opengl2.h"
#define OGL_INIT ImGui_ImplOpenGL2_Init
#define OGL_NEW_FRAME ImGui_ImplOpenGL2_NewFrame
#define OGL_RENDER_NEW_DATA ImGui_ImplOpenGL2_RenderDrawData
#endif

SDL_Window *g_Window = NULL;
SDL_GLContext g_GLContext = NULL;

void main_loop(void *);

int main(int, char **) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    const char *glsl_version = "#version 100";

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#ifdef EMSCRIPTEN
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    g_Window = SDL_CreateWindow("Dear ImGui SDL example", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, 600, 400, window_flags);
    g_GLContext = SDL_GL_CreateContext(g_Window);
    if (!g_GLContext) {
        fprintf(stderr, "Failed to initialize GL context!\n");
        return 1;
    }
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(FONT_PATH, 16.0f);
    io.IniFilename = NULL;
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(g_Window, g_GLContext);
#ifdef EMSCRIPTEN
    OGL_INIT(glsl_version);
    emscripten_set_main_loop_arg(main_loop, NULL, 0, true);
#else
    OGL_INIT();
    bool done = false;
    while (!done)
        main_loop(NULL);
#endif
}

void main_loop(void *arg) {
    ImGuiIO &io = ImGui::GetIO();
    IM_UNUSED(arg);

    static ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            exit(0);
    }

    OGL_NEW_FRAME();
    ImGui_ImplSDL2_NewFrame(g_Window);
    ImGui::NewFrame();

    {
        static float f = 0.0f;
        static int counter = 0;
        static bool open = true;
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(600, 400));

        ImGui::Begin("Hello, world!", &open,
                     ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoResize);

        ImGui::Text("This is some useful text.");

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float *)&clear_color);
        ImGui::PushItemWidth(100.0f);
        if (ImGui::Button("Button"))
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::End();
    }

    ImGui::Render();
    SDL_GL_MakeCurrent(g_Window, g_GLContext);
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    OGL_RENDER_NEW_DATA(ImGui::GetDrawData());
    SDL_GL_SwapWindow(g_Window);
}
