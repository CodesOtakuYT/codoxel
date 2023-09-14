cmake_minimum_required(VERSION 3.27.4)
project(codoxel VERSION 0.1.0 LANGUAGES C)

include(cmake/CPM.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

CPMAddPackage("gh:KhronosGroup/Vulkan-Headers#4f51aac")
CPMAddPackage("gh:recp/cglm#5090788")
CPMAddPackage(
    NAME volk
    GIT_REPOSITORY https://github.com/zeux/volk.git
    GIT_TAG 623a550
    DOWNLOAD_ONLY YES
)
CPMAddPackage(
  NAME VulkanMemoryAllocator
  GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
  GIT_TAG 2f382df
  # OPTIONS
  #   ""
)
CPMAddPackage("gh:libsdl-org/SDL#859dc14")

if(volk_ADDED)
    add_library(volk STATIC)
    target_sources(volk PRIVATE ${volk_SOURCE_DIR}/volk.c)
    target_include_directories(volk PUBLIC ${volk_SOURCE_DIR})
    target_link_libraries(volk PUBLIC Vulkan::Headers)
    target_compile_definitions(volk PUBLIC VK_NO_PROTOTYPES)
endif()

# Engine
add_library(${PROJECT_NAME} SHARED)
target_sources(${PROJECT_NAME} PRIVATE engine/src/engine.c)
target_include_directories(${PROJECT_NAME} PRIVATE engine/include)
target_compile_features(${PROJECT_NAME} PRIVATE c_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE
  -Weverything
  -Werror
  -ferror-limit=1
  -Wno-c++98-compat
  -Wno-declaration-after-statement
  -Wno-cast-function-type-strict
)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(${PROJECT_NAME} PRIVATE cglm volk GPUOpen::VulkanMemoryAllocator SDL3::SDL3)

# Editor
# add_library(${PROJECT_NAME}_editor SHARED)
# target_sources(${PROJECT_NAME}_editor PRIVATE editor/src/editor.c)
# target_compile_features(${PROJECT_NAME}_editor PRIVATE c_std_23)
# target_compile_options(${PROJECT_NAME}_editor PRIVATE
#   -Weverything
#   -Werror
#   -ferror-limit=1
#   -Wno-c++98-compat
#   -Wno-declaration-after-statement
#   -Wno-cast-function-type-strict
# )


# Launcher
add_executable(${PROJECT_NAME}_launcher)
target_sources(${PROJECT_NAME}_launcher PRIVATE src/main.c)
target_compile_features(${PROJECT_NAME}_launcher PRIVATE c_std_23)
target_compile_options(${PROJECT_NAME}_launcher PRIVATE
  -Weverything
  -Werror
  -ferror-limit=1
  -Wno-c++98-compat
  -Wno-declaration-after-statement
  -Wno-cast-function-type-strict
)
target_include_directories(${PROJECT_NAME}_launcher PRIVATE engine/include)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)