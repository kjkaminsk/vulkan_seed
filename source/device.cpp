#include <vulkan/vulkan.h>
#include <exception>
#include <vector>

#include "context.h"
#include "vulkan_instance.h"
//#include "validation_layers.h"

uint32_t queue_family = UINT32_MAX;

void create_device(Context& ctx)
{
    VkDeviceQueueCreateInfo queueCreateInfo = { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO };
    queueCreateInfo.queueFamilyIndex = queue_family;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = { VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO };
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    // not required for new vulkna implementations (instance level validation layers handle all now)
    //if (ctx.enableValidationLayers) {
    //    auto& validation_layers = get_validation_layers();
    //    createInfo.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
    //    createInfo.ppEnabledLayerNames = validation_layers.data();
    //}
    //else {
    //    createInfo.enabledLayerCount = 0;
    //}

    if (vkCreateDevice(ctx.physicalDevice, &createInfo, nullptr, &ctx.device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(ctx.device, queue_family, 0, &ctx.queue);
}

void cleanup_device(Context& ctx)
{
    vkDestroyDevice(ctx.device, nullptr);
}

bool isDeviceSuitable(VkPhysicalDevice device)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    uint32_t i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT)) {
            queue_family = i;
            break;
        }
        i++;
    }

    return queue_family != UINT32_MAX;
}

void pickPhysicalDevice(Context& ctx)
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
        if (isDeviceSuitable(device)) {
            ctx.physicalDevice = device;
            break;
        }
    }

    if (ctx.physicalDevice == nullptr) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

