# pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "context.h"

typedef struct Graphics_Pass_ {
    VkPipelineLayout p_layout;
    VkShaderModule vert;
    VkShaderModule frag;
    VkPipeline pipeline;
    VkRenderPass render_pass;
    std::vector<VkFramebuffer> swap_chain_framebuffers;
    std::vector<VkCommandBuffer> cmd_buffers;
} Graphics_Pass;

void create_graphics_pass(Context& ctx, Graphics_Pass& pass);
void destroy_graphics_pass(Context& ctx, Graphics_Pass& pass);
