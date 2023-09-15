#pragma once

#include <renderer.h>
#ifdef _WIN32
#define DLLEXPORT __declspec( dllexport )
#else
#define DLLEXPORT __attribute__ ((visibility("default")))
#endif

typedef struct {
  Renderer renderer;
} Engine;


typedef void(*PFN_engineCreate)(Engine *engine);
DLLEXPORT void engineCreate(Engine *engine);
DLLEXPORT void engineDestroy(Engine *engine);
