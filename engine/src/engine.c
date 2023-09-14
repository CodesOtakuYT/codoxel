#include <engine.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <volk.h>
#include <cglm/cglm.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#pragma clang diagnostic pop

[[noreturn]] static void fatalSdl() {
  fprintf(stderr, "%s\n", SDL_GetError());
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Error", SDL_GetError(),
                           NULL);
  exit(EXIT_FAILURE);
}

void engineInit() {
  if (SDL_Init(SDL_INIT_EVERYTHING))
    fatalSdl();
  if (SDL_Vulkan_LoadLibrary(NULL))
    fatalSdl();
  vkGetInstanceProcAddr =
      (PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr();
  if (!vkGetInstanceProcAddr)
    fatalSdl();
  volkInitializeCustom(vkGetInstanceProcAddr);

  Uint32 vulkan_version;
  if (vkEnumerateInstanceVersion(&vulkan_version) == VK_SUCCESS) {
    printf("Vulkan API %d.%d.%d\n", VK_API_VERSION_MAJOR(vulkan_version),
           VK_API_VERSION_MINOR(vulkan_version),
           VK_API_VERSION_PATCH(vulkan_version));
  }
}
