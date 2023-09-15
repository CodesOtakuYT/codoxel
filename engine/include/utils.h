#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <SDL3/SDL.h>
#include <cglm/cglm.h>
#pragma clang diagnostic pop

[[noreturn]] static void fatalSdl() {
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s\n", SDL_GetError());
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Error", SDL_GetError(),
                           NULL);
#ifdef _DEBUG
  SDL_TriggerBreakpoint();
#endif
  exit(EXIT_FAILURE);
}
