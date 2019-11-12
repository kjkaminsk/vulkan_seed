#pragma once

#include <vulkan/vulkan.h>

#include "context.h"

void create_swap_chain(Context& ctx);
void cleanup_swap_chain(Context& ctx);
bool is_swap_chain_acceptable(Context& ctx, VkPhysicalDevice physical_device);
