#include <vulkan/vulkan.h>

#include "context.h"
#include "graphics_pass.h"
#include "errors.h"
#include "draw_frame.h"

void draw_frame(Context& ctx, Graphics_Pass& pass)
{
	vkWaitForFences(ctx.device, 1, &ctx.fence, VK_TRUE, UINT64_MAX);
	vkResetFences(ctx.device, 1, &ctx.fence);

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
	tif(FL, vkQueueSubmit(ctx.queue, 1, &submitInfo, ctx.fence));

	VkPresentInfoKHR presentInfo = { VK_STRUCTURE_TYPE_PRESENT_INFO_KHR };
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &ctx.rendering_complete;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &ctx.swap_chain;
	presentInfo.pImageIndices = &image_index;
	vkQueuePresentKHR(ctx.queue, &presentInfo);

	vkQueueWaitIdle(ctx.queue);
}
