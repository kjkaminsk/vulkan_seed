#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

typedef struct Context_
{
    uint32_t width;
    uint32_t height;

    GLFWwindow* window;
    VkSurfaceKHR surface;

    VkInstance instance;
    bool enableValidationLayers;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physical_device;
    VkDevice device;
    VkQueue queue;

    VkSwapchainKHR swap_chain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

} Context;

