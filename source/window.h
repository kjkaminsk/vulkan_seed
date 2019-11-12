#pragma once

#include <vector>

#include "context.h"
#include "graphics_pass.h"

void init_window(Context& ctx);
void cleanup_window(Context& ctx);
void create_surface_by_glfw(Context& ctx);
std::vector<const char*> get_required_glfw_extensions(Context& ctx);
void main_loop(Context& ctx, Graphics_Pass& pass);
