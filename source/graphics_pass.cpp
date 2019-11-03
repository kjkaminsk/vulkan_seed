#include <vulkan/vulkan.h>

#include <stdexcept>
#include <vector>

#include "context.h"
#include "errors.h"
#include "graphics_pipeline.h"

void create_graphics_pass(Context& ctx)
{
    create_graphics_pipeline(ctx);
}

void destroy_graphics_pass(Context& ctx)
{
    destroy_graphics_pipeline(ctx);
}