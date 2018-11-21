#pragma once

#include <SDL.h>
#include "imgui.h"

#define SDL_CHECK_ZERO_FATAL(CODE) {                                    \
    int result = (CODE);                                                \
    if (result != 0) {                                                  \
      SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,                     \
                      "[%s:%d]: %s = %d, %s", __FILE__, __LINE__,       \
                      #CODE, result, SDL_GetError());                   \
      return -1;                                                        \
    }                                                                   \
  };

#define SDL_CHECK_ZERO(CODE) {                                       \
    int result = (CODE);                                             \
    if (result != 0) {                                               \
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,                      \
                  "[%s:%d]: %s = %d, %s", __FILE__, __LINE__,        \
                  #CODE, result, SDL_GetError());                    \
    }                                                                \
  };

#define SDL_CHECK_NOTNULL(CODE) {                                    \
    if ((CODE) == NULL)                                              \
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,                      \
                  "[%s:%d]: %s = NULL, %s", __FILE__, __LINE__,      \
                  #CODE, SDL_GetError());                            \
  };

const int DEFAULT_WINDOW_WIDTH = 1280;
const int DEFAULT_WINDOW_HEIGHT = 720;

struct GameDebugInfo {
  float actual_delay_s;
  ulong actual_delay_perf;
  float framerate;
  float frame_length;
  float imgui_draw_time_s;
  ulong imgui_draw_time_perf;
};
struct GameUI {
  bool show_demo_window = true;
  ImVec4 clear_color = ImVec4(0.05f, 0.35f, 0.60f, 1.00f);
};
struct GameWorld {
  GameDebugInfo debug_info;
  GameUI ui;

  bool do_run = true;

  int target_framerate = 60;
  int frame_delay = 10;
};
