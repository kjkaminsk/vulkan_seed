#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct Context_
{
    int width;
    int height;

    GLFWwindow* window;
    VkSurfaceKHR surface;

    VkInstance instance;
    bool enableValidationLayers;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue queue;

} Context;

