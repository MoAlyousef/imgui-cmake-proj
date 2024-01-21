#include <SDL.h>
#include <SDL_video.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(EMSCRIPTEN)
#include "imgui_impl_opengl3.h"
#include <emscripten.h>
#elif defined(__ANDROID__)
#include "imgui_impl_opengl3.h"
#else
#include "imgui_impl_opengl2.h"
#endif

#if defined(EMSCRIPTEN) || defined(__ANDROID__)
#include <SDL_opengles2.h>
#define OGL_INIT ImGui_ImplOpenGL3_Init
#define OGL_NEW_FRAME ImGui_ImplOpenGL3_NewFrame
#define OGL_RENDER_NEW_DATA ImGui_ImplOpenGL3_RenderDrawData
#else
#include <SDL_opengl.h>
#define OGL_INIT ImGui_ImplOpenGL2_Init
#define OGL_NEW_FRAME ImGui_ImplOpenGL2_NewFrame
#define OGL_RENDER_NEW_DATA ImGui_ImplOpenGL2_RenderDrawData
#endif

#define FONT_PATH "fonts/FreeSans.ttf"

SDL_Window *g_Window = NULL;
SDL_GLContext g_GLContext = NULL;

void main_loop(void *);

void show_gui();

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
                                SDL_WINDOWPOS_CENTERED, 1200, 800, window_flags);
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
    return 0;
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

    show_gui();
    ImGui::Render();
    SDL_GL_MakeCurrent(g_Window, g_GLContext);
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    OGL_RENDER_NEW_DATA(ImGui::GetDrawData());
    SDL_GL_SwapWindow(g_Window);
}

void show_gui() {
    static bool show_hem_size_calc = false;
    static bool show_stereo_calc = false;
    ImGui::SetNextWindowSize(ImVec2(300, 200));
    ImGui::Begin("Main Menu");
    ImGui::Text("Choose a calculator:");
    ImGui::NewLine();
    ImGui::Checkbox("Hematoma Size Calculator", &show_hem_size_calc);
    ImGui::NewLine();
    ImGui::Separator();
    ImGui::NewLine();
    ImGui::Checkbox("Stereotactic Calculator", &show_stereo_calc);
    ImGui::End();

    if (show_hem_size_calc) {
        static double size = 0;
        static int x = 0;
        static int y = 0;
        static int z = 0;
        ImGui::SetNextWindowSize(ImVec2(400, 300));
        ImGui::Begin("Hematoma size calculator", &show_hem_size_calc);
        ImGui::Text("Enter hematoma size:");
        ImGui::Separator();
        ImGui::NewLine();
        ImGui::InputInt("X axis", &x);
        ImGui::NewLine();
        ImGui::InputInt("Y axis", &y);
        ImGui::NewLine();
        ImGui::InputInt("Z axis", &z);
        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();

        if (ImGui::Button("Calculate"))
            size = static_cast<double>(x + y + z) / 3;
        ImGui::NewLine();
        ImGui::Text("Size = %lf", size);
        ImGui::NewLine();
        ImGui::End();
    }

    if (show_stereo_calc) {
        static int ring = 0;
        static int arc = 0;
        static int e_x = 0;
        static int e_y = 0;
        static int e_z = 0;
        static int t_x = 0;
        static int t_y = 0;
        static int t_z = 0;
        ImGui::SetNextWindowSize(ImVec2(480, 380));
        ImGui::Begin("Stereotactic calculator", &show_stereo_calc);
        ImGui::Text("Enter Axes:");
        ImGui::Separator();
        ImGui::NewLine();
        ImGui::PushItemWidth(150);
        ImGui::InputInt("Entry X", &e_x);
        ImGui::SameLine(200, 50);
        ImGui::InputInt("Target X", &t_x);
        ImGui::NewLine();
        ImGui::InputInt("Entry Y", &e_y);
        ImGui::SameLine(200, 50);
        ImGui::InputInt("Target Y", &t_y);
        ImGui::NewLine();
        ImGui::InputInt("Entry Z", &e_z);
        ImGui::SameLine(200, 50);
        ImGui::InputInt("Target Z", &t_z);
        ImGui::NewLine();
        ImGui::Separator();
        ImGui::NewLine();
        ImGui::PopItemWidth();

        if (ImGui::Button("Calculate")) {
            double Dx = t_x - e_x;
            double Dy = t_y - e_y;
            double Dz = t_z - e_z;
            double Dysq = pow(Dy, 2.0);
            double Dzsq = pow(Dz, 2.0);
            double M1M2 = Dysq + Dzsq;
            double rootcal = sqrt(M1M2);
            arc = 180.00 - (atan2(rootcal, Dx) * 180 / M_PI);
            ring = (atan(Dy / Dz) * 180 / M_PI) + 90;
        }
        ImGui::NewLine();
        ImGui::Text("Ring = %d", ring);
        ImGui::NewLine();
        ImGui::Text("Arc = %d", arc);
        ImGui::NewLine();
        ImGui::End();
    }
}
