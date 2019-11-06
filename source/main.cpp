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

void draw(Context& ctx, Graphics_Pass& pass)
{
    uint32_t image_index;
    vkAcquireNextImageKHR(ctx.device, ctx.swap_chain, UINT64_MAX, ctx.image_acquired, nullptr, &image_index);

    VkSemaphore waitSemaphores[] = { ctx.image_acquired };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSubmitInfo submitInfo = { VK_STRUCTURE_TYPE_SUBMIT_INFO };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &pass.cmd_buffers[image_index];
    VkSemaphore signalSemaphores[] = { ctx.rendering_complete };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    tif(FL, vkQueueSubmit(ctx.queue, 1, &submitInfo, nullptr));
}

void main_loop(Context& ctx, Graphics_Pass& pass)
{
    while (!glfwWindowShouldClose(ctx.window))
    {
        glfwPollEvents();

        draw(ctx, pass);
    }
}

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
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
