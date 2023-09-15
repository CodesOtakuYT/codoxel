#include <renderer.h>

static void checkVk(VkResult result) {
  if (result < VK_SUCCESS) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s%d\n", "Vulkan Error: ", result);
#ifdef _DEBUG
    SDL_TriggerBreakpoint();
#endif
    exit(EXIT_FAILURE);
  }
}

void rendererCreate(Renderer *renderer, RendererCreateInfo *createInfo) {
  (void)createInfo;
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

  VkInstance instance;
  {
    Uint32 extensions_count;
    if (!SDL_Vulkan_GetInstanceExtensions(&extensions_count, NULL))
      fatalSdl();
    const char **extensions = SDL_malloc(extensions_count * sizeof(char *));
    if (!SDL_Vulkan_GetInstanceExtensions(&extensions_count, extensions))
      fatalSdl();

    checkVk(vkCreateInstance(
        &(VkInstanceCreateInfo){
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo =
                &(VkApplicationInfo){
                    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                    .apiVersion = VK_API_VERSION_1_3,
                },
            .enabledExtensionCount = extensions_count,
            .ppEnabledExtensionNames = extensions,
        },
        NULL, &instance));
    volkLoadInstanceOnly(instance);
  }

  SDL_Window *window = SDL_CreateWindow(
      createInfo->title, createInfo->logical_dimensions[0],
      createInfo->logical_dimensions[1], SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN);
  if (!window)
    fatalSdl();

  VkSurfaceKHR surface;
  if (!SDL_Vulkan_CreateSurface(window, instance, &surface))
    fatalSdl();

  VkPhysicalDevice physical_device;
  checkVk(vkEnumeratePhysicalDevices(instance, &(Uint32){1}, &physical_device));

  VkDevice device;
  checkVk(vkCreateDevice(
      physical_device,
      &(VkDeviceCreateInfo){
          .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
          .queueCreateInfoCount = 1,
          .pQueueCreateInfos =
              &(VkDeviceQueueCreateInfo){
                  .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                  .queueCount = 1,
                  .queueFamilyIndex = 0,
                  .pQueuePriorities = &(float){0.0},
              },
          .enabledExtensionCount = 1,
          .ppEnabledExtensionNames =
              &(const char *){VK_KHR_SWAPCHAIN_EXTENSION_NAME},
      },
      NULL, &device));
  volkLoadDevice(device);

  VmaAllocator vma_allocator;
  checkVk(vmaCreateAllocator(
      &(VmaAllocatorCreateInfo){
          .vulkanApiVersion = VK_API_VERSION_1_3,
          .physicalDevice = physical_device,
          .device = device,
          .instance = instance,
          .pVulkanFunctions =
              &(VmaVulkanFunctions){
                  .vkGetPhysicalDeviceProperties =
                      vkGetPhysicalDeviceProperties,
                  .vkGetPhysicalDeviceMemoryProperties =
                      vkGetPhysicalDeviceMemoryProperties,
                  .vkAllocateMemory = vkAllocateMemory,
                  .vkFreeMemory = vkFreeMemory,
                  .vkMapMemory = vkMapMemory,
                  .vkUnmapMemory = vkUnmapMemory,
                  .vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges,
                  .vkInvalidateMappedMemoryRanges =
                      vkInvalidateMappedMemoryRanges,
                  .vkBindBufferMemory = vkBindBufferMemory,
                  .vkBindImageMemory = vkBindImageMemory,
                  .vkGetBufferMemoryRequirements =
                      vkGetBufferMemoryRequirements,
                  .vkGetImageMemoryRequirements = vkGetImageMemoryRequirements,
                  .vkCreateBuffer = vkCreateBuffer,
                  .vkDestroyBuffer = vkDestroyBuffer,
                  .vkCreateImage = vkCreateImage,
                  .vkDestroyImage = vkDestroyImage,
                  .vkCmdCopyBuffer = vkCmdCopyBuffer,
                  .vkGetBufferMemoryRequirements2KHR =
                      vkGetBufferMemoryRequirements2,
                  .vkGetImageMemoryRequirements2KHR =
                      vkGetImageMemoryRequirements2,
                  .vkBindBufferMemory2KHR = vkBindBufferMemory2,
                  .vkBindImageMemory2KHR = vkBindImageMemory2,
                  .vkGetPhysicalDeviceMemoryProperties2KHR =
                      vkGetPhysicalDeviceMemoryProperties2,
                  .vkGetDeviceBufferMemoryRequirements =
                      vkGetDeviceBufferMemoryRequirements,
                  .vkGetDeviceImageMemoryRequirements =
                      vkGetDeviceImageMemoryRequirements,
              },
      },
      &vma_allocator));

  SDL_ShowWindow(window);

  renderer->instance = instance;
  renderer->physical_device = physical_device;
  renderer->device = device;
  renderer->vma_allocator = vma_allocator;
}

void rendererDestroy(Renderer *renderer) {
  vmaDestroyAllocator(renderer->vma_allocator);
  vkDestroyDevice(renderer->device, NULL);
  vkDestroySurfaceKHR(renderer->instance, renderer->surface, NULL);
  SDL_DestroyWindow(renderer->window);
  vkDestroyInstance(renderer->instance, NULL);
  SDL_Vulkan_UnloadLibrary();
}
