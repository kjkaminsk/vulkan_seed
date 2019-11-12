#pragma once

#include <vulkan/vulkan.h>

#include <string>
#include <stdexcept>

#define FL __FILE__, __LINE__
void tif(char * file_line, int line, VkResult result);
std::string get_result_name(VkResult result);
