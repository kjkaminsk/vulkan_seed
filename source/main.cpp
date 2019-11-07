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
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &ctx.rendering_complete;
    tif(FL, vkQueueSubmit(ctx.queue, 1, &submitInfo, nullptr));

    VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &ctx.rendering_complete;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &ctx.swap_chain;
    presentInfo.pImageIndices = &image_index;
    vkQueuePresentKHR(ctx.queue, &presentInfo);

    vkQueueWaitIdle(ctx.queue);
}

void main_loop(Context& ctx, Graphics_Pass& pass)
{
    while (!glfwWindowShouldClose(ctx.window))
    {
        glfwPollEvents();

        draw(ctx, pass);
    }
    vkDeviceWaitIdle(ctx.device);
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
