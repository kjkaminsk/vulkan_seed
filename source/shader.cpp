#include <vulkan/vulkan.h>

#include <stdexcept>
#include <vector>
#include <fstream>
#include <string>

#include "context.h"
#include "errors.h"

std::vector<char> read_file(const std::string& file_name)
{
    std::ifstream file(file_name, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkShaderModule create_shader_module(Context& ctx, const std::string& file_name)
{
    auto code = read_file(file_name);
    VkShaderModuleCreateInfo createInfo = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    tif(FL, vkCreateShaderModule(ctx.device, &createInfo, nullptr, &shaderModule));

    return shaderModule;
}
