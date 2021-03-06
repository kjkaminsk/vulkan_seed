#include "context.h"
#include "vulkan_instance.h"
#include "surface.h"
#include "validation_layers.h"
#include "device.h"
#include "swap_chain.h"
#include "synchronization.h"

void init_vulkan(Context& ctx)
{
    create_instance(ctx);
    create_debug_messenger(ctx);
    create_surface(ctx);
    choose_gpu(ctx);
    create_device(ctx);
    create_swap_chain(ctx);
    create_semaphores(ctx);
    create_fence(ctx);
}

void cleanup_vulkan(Context& ctx)
{
    cleanup_fence(ctx);
    cleanup_semaphores(ctx);
    cleanup_swap_chain(ctx);
    cleanup_device(ctx);
    cleanup_validation_layers(ctx);
    cleanup_surface(ctx);
    cleanup_instance(ctx);
}
