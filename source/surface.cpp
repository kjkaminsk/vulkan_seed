#include "context.h"
#include "errors.h"
#include "surface.h"
#include "window.h"

void create_surface(Context& ctx) {
    //VkWin32SurfaceCreateInfoKHR createInfo = {};
    //createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    //createInfo.hwnd = glfwGetWin32Window(ctx.window);
    //createInfo.hinstance = GetModuleHandle(nullptr);
    //if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
    //    throw std::runtime_error("failed to create window surface!");
    //}
    //tif(FL, glfwCreateWindowSurface(ctx.instance, ctx.window, nullptr, &ctx.surface));

    create_surface_by_glfw(ctx);
}

void cleanup_surface(Context& ctx)
{
    vkDestroySurfaceKHR(ctx.instance, ctx.surface, nullptr);
}
