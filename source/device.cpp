#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <set>

#include "context.h"
#include "errors.h"
#include "vulkan_instance.h"
#include "swap_chain.h"


const std::vector<const char*> rquired_device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void create_command_pool(Context& ctx)
{
    VkCommandPoolCreateInfo poolInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
    poolInfo.queueFamilyIndex = ctx.queue_family;

    tif(FL, vkCreateCommandPool(ctx.device, &poolInfo, nullptr, &ctx.cmd_pool));
}

void create_device(Context& ctx)
{
    VkDeviceQueueCreateInfo queueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
    queueCreateInfo.queueFamilyIndex = ctx.queue_family;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    // no features requested yet
    //VkPhysicalDeviceFeatures deviceFeatures = {};
    //createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(rquired_device_extensions.size());
    createInfo.ppEnabledExtensionNames = rquired_device_extensions.data();

    // providing validation layers is not required for new vulkan implementations (instance level handle them now)

    tif(FL, vkCreateDevice(ctx.physical_device, &createInfo, nullptr, &ctx.device));

    vkGetDeviceQueue(ctx.device, ctx.queue_family, 0, &ctx.queue);
    create_command_pool(ctx);
}

void cleanup_device(Context& ctx)
{
    vkDestroyCommandPool(ctx.device, ctx.cmd_pool, nullptr);
    vkDestroyDevice(ctx.device, nullptr);
}

bool are_extensions_supported(VkPhysicalDevice physical_device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> required(rquired_device_extensions.begin(), rquired_device_extensions.end());

    for (const auto& available : availableExtensions) {
        required.erase(available.extensionName);
    }

    return required.empty();
}

// Check common queue family support and record family index
bool is_common_queue_found(Context& ctx, VkPhysicalDevice physical_device)
{
    uint32_t queue_family = UINT32_MAX;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamilyCount, queueFamilies.data());

    uint32_t i = 0;
    for (const auto& queueFamily : queueFamilies) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, ctx.surface, &presentSupport);

        if (presentSupport && (queueFamily.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT))) {
            queue_family = i;
            break;
        }

        i++;
    }

    return queue_family != UINT32_MAX;
}

bool is_device_good_enough(Context& ctx, VkPhysicalDevice physical_device)
{
    return is_common_queue_found(ctx, physical_device)
           && are_extensions_supported(physical_device)
           && is_swap_chain_acceptable(ctx, physical_device);
}

void choose_gpu(Context& ctx)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(ctx.instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(ctx.instance, &deviceCount, devices.data());

    // take the first device that supports requirements
    for (const auto& device : devices) {
        if (is_device_good_enough(ctx, device)) {
            ctx.physical_device = device;
            break;
        }
    }

    if (ctx.physical_device == nullptr) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

