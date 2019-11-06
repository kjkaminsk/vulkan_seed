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
    // use only one queue for: graphics, compute, transfer and present
    uint32_t queue_family;
    VkDevice device;
    VkQueue queue;
    VkCommandPool cmd_pool;

    VkSwapchainKHR swap_chain;
    VkFormat swap_chain_format;
    //VkExtent2D swapChainExtent; 
    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    VkSemaphore image_acquired;
    VkSemaphore rendering_complete;
} Context;

void init_vulkan(Context& ctx);
void cleanup_vulkan(Context& ctx);
