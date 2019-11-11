#include <vulkan/vulkan.h>

#include <stdexcept>
#include <vector>

#include "context.h"
#include "errors.h"
#include "graphics_pipeline.h"
#include "graphics_pass.h"

void create_render_pass(Context& ctx, Graphics_Pass& pass) {
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = ctx.swap_chain_format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    tif(FL, vkCreateRenderPass(ctx.device, &renderPassInfo, nullptr, &pass.render_pass));
}

void create_framebuffers(Context& ctx, Graphics_Pass& pass)
{
    pass.swap_chain_framebuffers.resize(ctx.image_count);

    for (size_t i = 0; i < pass.swap_chain_framebuffers.size(); i++) {
        VkFramebufferCreateInfo framebufferInfo = { VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
        framebufferInfo.renderPass = pass.render_pass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &ctx.swapChainImageViews[i];
        framebufferInfo.width = ctx.width;
        framebufferInfo.height = ctx.height;
        framebufferInfo.layers = 1;

        tif(FL, vkCreateFramebuffer(ctx.device, &framebufferInfo, nullptr, &pass.swap_chain_framebuffers[i]));
    }
}

void create_command_buffers(Context& ctx, Graphics_Pass& pass)
{
    pass.cmd_buffers.resize(ctx.image_count);

    VkCommandBufferAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    allocInfo.commandPool = ctx.cmd_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)pass.cmd_buffers.size();

    tif(FL, vkAllocateCommandBuffers(ctx.device, &allocInfo, pass.cmd_buffers.data()));

    for (size_t i = 0; i < pass.cmd_buffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
        tif(FL, vkBeginCommandBuffer(pass.cmd_buffers[i], &beginInfo));

        VkRenderPassBeginInfo renderPassInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
        renderPassInfo.renderPass = pass.render_pass;
        renderPassInfo.framebuffer = pass.swap_chain_framebuffers[i];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = { ctx.width, ctx.height };

        VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(pass.cmd_buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(pass.cmd_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pass.pipeline);

        vkCmdDraw(pass.cmd_buffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(pass.cmd_buffers[i]);

        tif(FL, vkEndCommandBuffer(pass.cmd_buffers[i]));
    }

}

void create_graphics_pass(Context& ctx, Graphics_Pass& pass)
{
    create_render_pass(ctx, pass);
    create_framebuffers(ctx, pass);
    create_graphics_pipeline(ctx, pass);
    create_command_buffers(ctx, pass);
}

void destroy_graphics_pass(Context& ctx, Graphics_Pass& pass)
{
    vkFreeCommandBuffers(ctx.device, ctx.cmd_pool, (uint32_t)pass.cmd_buffers.size(), pass.cmd_buffers.data());
    destroy_graphics_pipeline(ctx, pass);
    for (auto framebuffer : pass.swap_chain_framebuffers) {
        vkDestroyFramebuffer(ctx.device, framebuffer, nullptr);
    }
    vkDestroyRenderPass(ctx.device, pass.render_pass, nullptr);
}