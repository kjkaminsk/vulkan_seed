#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

#include "context.h"
#include "window.h"
#include "vulkan_instance.h"
#include "surface.h"
#include "validation_layers.h"
#include "device.h"
#include "swap_chain.h"

void initVulkan(Context& ctx)
{
    create_instance(ctx);
    setupDebugMessenger(ctx);
    create_surface(ctx);
    pickPhysicalDevice(ctx);
    create_device(ctx);
    create_swap_chain(ctx);
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
    cleanup_swap_chain(ctx);
    cleanup_device(ctx);
    cleanup_validation_layers(ctx);
    cleanup_surface(ctx);
    cleanup_instance(ctx);
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
