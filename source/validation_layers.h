#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "context.h"

bool check_validation_layer_support();
void create_debug_messenger(Context& ctx);
void cleanup_validation_layers(Context& ctx);
void populate_debug_messenger_create_info(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
std::vector<const char*>& get_validation_layers();
