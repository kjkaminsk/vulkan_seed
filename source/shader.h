#pragma once

#include <vector>
#include <string>

#include "context.h"

VkShaderModule create_shader_module(Context& ctx, const std::string& file_name);
