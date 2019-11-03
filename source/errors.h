#pragma once

#include <string>

#define FL __FILE__, __LINE__
void tif(char * file_line, int line, VkResult result);
std::string get_result_name(VkResult result);
