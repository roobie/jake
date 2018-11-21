// dear imgui: standalone example application for SDL2 + OpenGL
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_sdl_opengl3/ folder**
// See imgui_impl_sdl.cpp for details.

// #include <stdio.h>
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "jake.h"
#include "jake_lib.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"

void Cleanup(SDL_Window* window, SDL_GLContext gl_context) {

  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  // Cleanup all the things we bound and allocated
  //shader.CleanUp();

  //glDisableVertexAttribArray(0);
  //glDeleteBuffers(1, vbo);
  //glDeleteVertexArrays(1, vao);
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void
printInfo() {
  int glmaj, glmin;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &glmaj);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &glmin);
  SDL_Log("GL Version: %d.%d", glmaj, glmin);
};

int
main(int, char**) {
  SDL_CHECK_ZERO_FATAL(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER));

  SDL_CHECK_ZERO(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
  SDL_CHECK_ZERO(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));
  SDL_CHECK_ZERO(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8));
  SDL_CHECK_ZERO(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2));
  SDL_CHECK_ZERO(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2));

  SDL_DisplayMode current;
  SDL_CHECK_ZERO(SDL_GetCurrentDisplayMode(0, &current));
  auto windowFlags = SDL_WINDOW_OPENGL; // |SDL_WINDOW_RESIZABLE;
  SDL_Window* window = SDL_CreateWindow("Jake :: Your friendly multiverse validator.",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                                        windowFlags);
  IM_ASSERT(window);

  auto renderer_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
  auto renderer = SDL_CreateRenderer(window, -1, renderer_flags);
  IM_ASSERT(renderer);

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  IM_ASSERT(gl_context);

  SDL_CHECK_ZERO(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));

  SDL_CHECK_ZERO(SDL_GL_SetSwapInterval(1));

	// Init GLEW
	// Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL2_Init();

  // Setup Style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsClassic();

  // Load Fonts
  // - If no fonts are loaded, dear imgui will use the default font.
  //   You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
  // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to
  //   select the font among multiple.
  // - If the file cannot be loaded, the function will return NULL. Please handle those errors
  //   in your application (e.g. use an assertion, or display an error and quit).
  // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture
  //   when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
  // - Read 'misc/fonts/README.txt' for more instructions and details.
  // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
  // IM_ASSERT(io.Fonts->AddFontDefault());
  // IM_ASSERT(io.Fonts->AddFontFromFileTTF("./imgui/misc/fonts/Roboto-Medium.ttf", 15.0f));
  IM_ASSERT(io.Fonts->AddFontFromFileTTF("./imgui/misc/fonts/Cousine-Regular.ttf", 15.0f));
  //IM_ASSERT(io.Fonts->AddFontFromFileTTF("./imgui/misc/fonts/DroidSans.ttf", 16.0f));
  //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
  //IM_ASSERT(io.Fonts->AddFontFromFileTTF("./imgui/misc/fonts/ProggyTiny.ttf", 15.0f));
  //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
  //IM_ASSERT(font != NULL);

  // GameWorld - the one and only
  static GameWorld world;

  printInfo();

  // Main loop
  while (world.do_run) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {

      ImGui_ImplSDL2_ProcessEvent(&event);

      switch (event.type) {
      case SDL_QUIT: world.do_run = false; break;
      case SDL_MOUSEMOTION:
        if (!io.WantCaptureMouse) {
        }
      case SDL_MOUSEWHEEL:
        if (!io.WantCaptureMouse) {
        }
      case SDL_MOUSEBUTTONDOWN:
        if (!io.WantCaptureMouse) {
        }
      case SDL_KEYDOWN:
        if (!io.WantCaptureKeyboard) {
          switch (event.key.keysym.sym) {
          case SDLK_q: world.do_run = false; break;
          }
        }
      }
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (world.ui.show_demo_window)
      ImGui::ShowDemoWindow(&world.ui.show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
      ImGui::Begin("DevInfo");

      // Display some text (you can use a format strings too)
      // ImGui::Text("This is some useful text.");
      // Edit bools storing our window open/close state
      ImGui::Checkbox("Demo Window", &world.ui.show_demo_window);

      // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit4("clear color", (float*)&world.ui.clear_color);

      // Buttons return true when clicked (most widgets return true when edited/activated)
      // if (ImGui::Button("Button"))
      // ImGui::SameLine();
      // ImGui::Text("counter = %d", counter);

      // world.debug_info.framerate = ImGui::GetIO().Framerate;
      world.debug_info.framerate = io.Framerate;
      world.debug_info.frame_length = 1000.0f / world.debug_info.framerate;
      ImGui::Text("Average %.1f ms/frame (%.1f FPS)",
                  world.debug_info.frame_length, world.debug_info.framerate);

      ImGui::Text("Delay (wanted/actual): %d ms/%.3f s",
                  world.frame_delay, world.debug_info.actual_delay_s);

      world.debug_info.imgui_draw_time_s = (float)world.debug_info.imgui_draw_time_perf / performance_frequency;
      ImGui::Text("ImGui draw time: %.3f s",
                  world.debug_info.imgui_draw_time_s);
      ImGui::End();
    }

    // Rendering
    COUNT_PERFORMANCE(world.debug_info.imgui_draw_time_perf, {
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        auto clear_color = world.ui.clear_color;
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        // glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
      });

    /** nogo
    SDL_Rect rect = {10, 10, 10, 10};
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
    */
    SDL_GL_SwapWindow(window);

    COUNT_PERFORMANCE(world.debug_info.actual_delay_perf, SDL_Delay(world.frame_delay));
    world.debug_info.actual_delay_s = (float)world.debug_info.actual_delay_perf / performance_frequency;
  }

  Cleanup(window, gl_context);

  return 0;
}
