#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "context.h"

void initWindow(Context& ctx)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    ctx.window = glfwCreateWindow(ctx.width, ctx.height, "Vulkan", nullptr, nullptr);
}

void cleanup_window(Context& ctx)
{
    glfwDestroyWindow(ctx.window);
    glfwTerminate();
}

