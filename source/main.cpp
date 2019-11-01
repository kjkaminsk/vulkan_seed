#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "context.h"
#include "vulkan_instance.h"
#include "validation_layers.h"
#include "device.h"

void initWindow(Context& ctx)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    ctx.window = glfwCreateWindow(ctx.width, ctx.height, "Vulkan", nullptr, nullptr);
}

void initVulkan(Context& ctx)
{
    create_instance(ctx);
    setupDebugMessenger(ctx);
    pickPhysicalDevice(ctx);
    create_device(ctx);
}

void mainLoop(Context& ctx)
{
    while (!glfwWindowShouldClose(ctx.window))
    {
        glfwPollEvents();
    }
}

void cleanup_vulkan(Context& ctx)
{
    cleanup_device(ctx);
    cleanup_validation_layers(ctx);
    cleanup_instance(ctx);
}

void cleanup_window(Context& ctx)
{
    glfwDestroyWindow(ctx.window);
    glfwTerminate();
}

int main()
{
    Context ctx = {};

    ctx.width = 800;
    ctx.height = 600;

    try
    {
        initWindow(ctx);
        initVulkan(ctx);
        mainLoop(ctx);
        cleanup_vulkan(ctx);
        cleanup_window(ctx);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
