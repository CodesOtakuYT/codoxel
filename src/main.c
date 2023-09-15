#include <engine.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  void *library = SDL_LoadObject("codoxel");
  if (!library)
    fatalSdl();
  Engine engine;
  PFN_engineCreate engineCreate =
      (PFN_engineCreate)SDL_LoadFunction(library, "engineCreate");
  PFN_engineCreate engineDestroy =
      (PFN_engineCreate)SDL_LoadFunction(library, "engineDestroy");
  if (!(engineCreate || engineDestroy))
    fatalSdl();
  engineCreate(&engine);
  engineDestroy(&engine);
  SDL_UnloadObject(library);
  return EXIT_SUCCESS;
}
