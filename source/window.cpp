#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

#include "context.h"
#include "errors.h"

void init_window(Context& ctx)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    ctx.window = glfwCreateWindow(ctx.width, ctx.height, "Vulkan", nullptr, nullptr);
}

void cleanup_window(Context& ctx)
{
    glfwDestroyWindow(ctx.window);
    glfwTerminate();
}

void create_surface_by_glfw(Context& ctx)
{
    tif(FL, glfwCreateWindowSurface(ctx.instance, ctx.window, nullptr, &ctx.surface));
}

std::vector<const char*> get_required_glfw_extensions(Context& ctx)
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
}
