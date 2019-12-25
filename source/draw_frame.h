#pragma once

#include "context.h"
#include "graphics_pass.h"

void draw_frame(Context& ctx, Graphics_Pass& pass);
void recreate_swap_chain(Context& ctx, Graphics_Pass& pass);
