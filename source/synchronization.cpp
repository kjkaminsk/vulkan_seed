#include <vulkan/vulkan.h>

#include <vector>

#include "context.h"
#include "errors.h"
#include "synchronization.h"

void create_fence(Context& ctx)
{
    VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    tif(FL, vkCreateFence(ctx.device, &fenceInfo, nullptr, &ctx.fence));
}

void cleanup_fence(Context& ctx)
{
    vkDestroyFence(ctx.device, ctx.fence, nullptr);
}

void create_semaphores(Context& ctx)
{
    VkSemaphoreCreateInfo semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

	tif(FL, vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr, &ctx.image_acquired));
	ctx.rendering_complete.resize(ctx.image_count);
	for (auto& semaphore : ctx.rendering_complete) {
		tif(FL, vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr, &semaphore));
	}
}

void cleanup_semaphores(Context& ctx)
{
	vkDestroySemaphore(ctx.device, ctx.image_acquired, nullptr);
	for (auto& semaphore : ctx.rendering_complete) {
		vkDestroySemaphore(ctx.device, semaphore, nullptr);
	}
}
