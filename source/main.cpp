//#define GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

#include "context.h"
#include "errors.h"

//#include "window.h"
#include "window_manual.h"

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
        //create_window_glfw(ctx);
        create_window_manual(ctx);

        init_vulkan(ctx);
        create_graphics_pass(ctx, pass);

        //main_loop_glfw(ctx, pass);
        main_loop_manual(ctx, pass);

        destroy_graphics_pass(ctx, pass);
        cleanup_vulkan(ctx);

        //cleanup_window_glfw(ctx);
        cleanup_window_manual(ctx);
    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
