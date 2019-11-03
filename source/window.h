#pragma once

#include <vector>

void init_window(Context& ctx);
void cleanup_window(Context& ctx);
void create_surface_by_glfw(Context& ctx);
std::vector<const char*> get_required_glfw_extensions(Context& ctx);
