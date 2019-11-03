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

    VkRenderPassCreateInfo renderPassInfo = { VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO };
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    tif(FL, vkCreateRenderPass(ctx.device, &renderPassInfo, nullptr, &pass.render_pass));
}

void create_framebuffers(Context& ctx, Graphics_Pass& pass)
{
    pass.swap_chain_framebuffers.resize(ctx.swapChainImageViews.size());

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

void create_graphics_pass(Context& ctx, Graphics_Pass& pass)
{
    create_render_pass(ctx, pass);
    create_framebuffers(ctx, pass);
    create_graphics_pipeline(ctx, pass);
}

void destroy_graphics_pass(Context& ctx, Graphics_Pass& pass)
{
    destroy_graphics_pipeline(ctx, pass);
    for (auto framebuffer : pass.swap_chain_framebuffers) {
        vkDestroyFramebuffer(ctx.device, framebuffer, nullptr);
    }
    vkDestroyRenderPass(ctx.device, pass.render_pass, nullptr);
}