#include <engine.h>
#include <utils.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <cglm/cglm.h>
#include <SDL3/SDL.h>
#pragma clang diagnostic pop

void engineCreate(Engine *engine) {
  if (SDL_Init(SDL_INIT_EVERYTHING))
    fatalSdl();
  Renderer renderer;
  rendererCreate(&renderer,
                 &(RendererCreateInfo){.title = "Codoxel", {800, 600}});

  engine->renderer = renderer;
}

void engineDestroy(Engine *engine) { rendererDestroy(&engine->renderer); }
