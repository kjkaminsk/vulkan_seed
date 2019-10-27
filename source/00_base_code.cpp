#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

typedef struct Context_ {
    int width;
    int height;

    GLFWwindow* window;
} Context;

void initWindow(Context& ctx)
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    ctx.window = glfwCreateWindow(ctx.width, ctx.height, "Vulkan", nullptr, nullptr);
}

void initVulkan(Context& ctx)
{

}

void mainLoop(Context& ctx)
{
    while (!glfwWindowShouldClose(ctx.window))
    {
        glfwPollEvents();
    }
}

void cleanup(Context& ctx)
{
    glfwDestroyWindow(ctx.window);
    glfwTerminate();
}

void run(Context& ctx)
{
    initWindow(ctx);
    initVulkan(ctx);
    mainLoop(ctx);
    cleanup(ctx);
}

int main()
{
    Context ctx;

    ctx.width = 800;
    ctx.height = 600;

    try
    {
        run(ctx);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
