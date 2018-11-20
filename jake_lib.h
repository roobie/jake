
#include <SDL.h>
#include "jake.h"
#include "imgui.h"

/**
   Divide the result of `COUNT_PERFORMANCE` with this here value
   to compute the time taken in seconds.
 */
static ulong performance_frequency = SDL_GetPerformanceFrequency();

/**
   VAR must be a `ulong` variable.
   CODE is the code to count the performance of.
 */
#define COUNT_PERFORMANCE(VAR, CODE) {               \
    ulong temp_begin = SDL_GetPerformanceCounter();  \
    (CODE);                                          \
    VAR = SDL_GetPerformanceCounter() - temp_begin;  \
};
