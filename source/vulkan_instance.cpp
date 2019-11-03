#include <vulkan/vulkan.h>

#include <stdexcept>
#include <vector>

#include "context.h"
#include "errors.h"
#include "vulkan_instance.h"
#include "validation_layers.h"
#include "window.h"

std::vector<const char*> get_required_extensions(Context& ctx)
{
    auto extensions = get_required_glfw_extensions(ctx);

    if (ctx.enable_validation_layers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void cleanup_instance(Context& ctx)
{
    vkDestroyInstance(ctx.instance, nullptr);
}

void create_instance(Context& ctx)
{
    ctx.enable_validation_layers = true;
    //ctx.enable_validation_layers = false;

    if (ctx.enable_validation_layers && !check_validation_layer_support()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
    appInfo.pApplicationName = "Vulkan Seed";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = get_required_extensions(ctx);
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (ctx.enable_validation_layers) {
        auto& validation_layers = get_validation_layers();
        createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        createInfo.ppEnabledLayerNames = validation_layers.data();

        populate_debug_messenger_create_info(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }

    tif(FL, vkCreateInstance(&createInfo, nullptr, &ctx.instance));
}

