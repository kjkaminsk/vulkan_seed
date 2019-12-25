#include "context.h"
#include "errors.h"
#include "surface.h"
//#include "window.h"

void create_surface(Context& ctx) {
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = ctx.window_manual;
    createInfo.hinstance = GetModuleHandle(nullptr);
    tif(FL, vkCreateWin32SurfaceKHR(ctx.instance, &createInfo, nullptr, &ctx.surface));

    //tif(FL, glfwCreateWindowSurface(ctx.instance, ctx.window, nullptr, &ctx.surface));
    //create_surface_by_glfw(ctx);
}

void cleanup_surface(Context& ctx)
{
    vkDestroySurfaceKHR(ctx.instance, ctx.surface, nullptr);
}
