#include <vulkan/vulkan.h>

#include <stdexcept>
#include <vector>

#include "context.h"
#include "errors.h"
#include "shader.h"

void create_graphics_pipeline(Context& ctx)
{
    VkShaderModule vertShaderModule = create_shader_module(ctx, "../shaders/triangle.vert.spv");
    VkShaderModule fragShaderModule = create_shader_module(ctx, "../shaders/triangle.frag.spv");

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = { VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    vkDestroyShaderModule(ctx.device, fragShaderModule, nullptr);
    vkDestroyShaderModule(ctx.device, vertShaderModule, nullptr);
}

void destroy_graphics_pipeline(Context& ctx)
{

}
