#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

#include "context.h"
#include "errors.h"
#include "window.h"
#include "graphics_pass.h"
#include "draw_frame.h"


int main()
{
    Context ctx = {};
    ctx.width = 800;
    ctx.height = 600;

    Graphics_Pass pass = {};

    try
    {
        init_window(ctx);
        init_vulkan(ctx);
        create_graphics_pass(ctx, pass);
        main_loop(ctx, pass);
        destroy_graphics_pass(ctx, pass);
        cleanup_vulkan(ctx);
        cleanup_window(ctx);
    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
