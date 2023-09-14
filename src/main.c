#include <engine.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

int main() {
#ifdef _WIN32
  HANDLE const library = LoadLibraryW(L"codoxel.dll");
  if (!library) {
    fprintf(stderr, "Couldn't load codoxel.dll");
    return EXIT_FAILURE;
  }
  PFN_engineInit engineInit =
      (PFN_engineInit)GetProcAddress(library, "engineInit");
  if (!engineInit) {
    fprintf(stderr, "Couldn't load engineInit function");
    return EXIT_FAILURE;
  }
#else
  void *library = dlopen("codoxel.so", RTLD_LAZY | RTLD_LOCAL);
  if (!library) {
    perror("Couldn't load codoxel.so");
    return EXIT_FAILURE;
  }
  PFN_engineInit engineInit = (PFN_engineInit)dlsym(library, "engineInit");
  if (!engineInit) {
    fprintf(stderr, "Couldn't load engineInit function");
    return EXIT_FAILURE;
  }
#endif
  engineInit();
  return EXIT_SUCCESS;
}
