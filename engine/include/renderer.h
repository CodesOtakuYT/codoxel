#pragma once
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#define VMA_VULKAN_VERSION 1003000 // Vulkan 1.3

#include <utils.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#include <vulkan/vulkan.h>
#include <volk.h>
#include <SDL3/SDL_vulkan.h>
#include <vk_mem_alloc.h>
#pragma clang diagnostic pop

typedef struct {
  VkInstance instance;
  SDL_Window *window;
  VkSurfaceKHR surface;
  VkPhysicalDevice physical_device;
  VkDevice device;
  VmaAllocator vma_allocator;
} Renderer;

typedef struct {
  const char* title;
  ivec2 logical_dimensions;
} RendererCreateInfo;

void rendererCreate(Renderer *renderer, RendererCreateInfo *createInfo);
void rendererDestroy(Renderer *renderer);
