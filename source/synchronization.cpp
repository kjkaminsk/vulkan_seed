#include <vulkan/vulkan.h>

#include <vector>

#include "context.h"
#include "errors.h"
#include "synchronization.h"

void create_semaphores(Context& ctx)
{
    VkSemaphoreCreateInfo semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

    tif(FL, vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr, &ctx.image_acquired));
    tif(FL, vkCreateSemaphore(ctx.device, &semaphoreInfo, nullptr, &ctx.rendering_complete));
}

void cleanup_semaphores(Context& ctx)
{
    vkDestroySemaphore(ctx.device, ctx.image_acquired, nullptr);
    vkDestroySemaphore(ctx.device, ctx.rendering_complete, nullptr);
}
