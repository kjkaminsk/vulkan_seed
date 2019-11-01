#pragma once

#include <vector>

bool checkValidationLayerSupport();
void setupDebugMessenger(Context& ctx);
void cleanup_validation_layers(Context& ctx);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
std::vector<const char*>& get_validation_layers();
