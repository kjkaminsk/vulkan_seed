//#define GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>
//
//#include <vector>
//
//#include "context.h"
//#include "graphics_pass.h"
//#include "errors.h"
//#include "draw_frame.h"
//
//void create_window_glfw(Context& ctx)
//{
//    glfwInit();
//    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//    ctx.window = glfwCreateWindow(ctx.width, ctx.height, "Vulkan_Seed", nullptr, nullptr);
//}
//
//void cleanup_window_glfw(Context& ctx)
//{
//    glfwDestroyWindow(ctx.window);
//    glfwTerminate();
//}
//
//void create_surface_by_glfw(Context& ctx)
//{
//    tif(FL, glfwCreateWindowSurface(ctx.instance, ctx.window, nullptr, &ctx.surface));
//}
//
//std::vector<const char*> get_required_glfw_extensions(Context& ctx)
//{
//    uint32_t glfwExtensionCount = 0;
//    const char** glfwExtensions;
//    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//
//    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
//
//    return extensions;
//}
//
//void main_loop_glfw(Context& ctx, Graphics_Pass& pass)
//{
//    while (!glfwWindowShouldClose(ctx.window))
//    {
//        glfwPollEvents();
//
//        draw_frame(ctx, pass);
//    }
//    vkDeviceWaitIdle(ctx.device);
//}
//
