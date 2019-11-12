#pragma once

#include "context.h"
#include "graphics_pass.h"

void create_window_manual(Context& ctx);
void cleanup_window_manual(Context& ctx);
//void create_surface_manual(Context& ctx);
void main_loop_manual(Context& ctx, Graphics_Pass& pass);
