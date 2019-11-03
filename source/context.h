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
    bool enable_validation_layers;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physical_device;
    VkDevice device;
    VkQueue queue;

    VkSwapchainKHR swap_chain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;

} Context;

