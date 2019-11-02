#include <vulkan/vulkan.h>

#include <vector>
#include <algorithm>

#include "context.h"
#include "swap_chain.h"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

// query is used when physical devices are checked before choosing
// and after creating device (so always a physical_device handle is passed)
SwapChainSupportDetails querySwapChainSupport(Context& ctx, VkPhysicalDevice physical_device)
{
    SwapChainSupportDetails details;

    // get surface capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, ctx.surface, &details.capabilities);

    // get surface formats
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, ctx.surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, ctx.surface, &formatCount, details.formats.data());
    }

    // get surface present modes
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, ctx.surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.present_modes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, ctx.surface, &presentModeCount, details.present_modes.data());
    }

    return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(Context& ctx, const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }
    else {
        VkExtent2D actualExtent = { ctx.width, ctx.height };

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

// this check is used when physical devices are examined, so the context does not have physical_device chosen yet
bool is_swap_chain_acceptable(Context& ctx, VkPhysicalDevice physical_device)
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(ctx, physical_device);
    return !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();
}

void cleanup_swap_chain(Context& ctx)
{
    vkDestroySwapchainKHR(ctx.device, ctx.swap_chain, nullptr);
}

void create_swap_chain(Context& ctx)
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(ctx, ctx.physical_device);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.present_modes);
    VkExtent2D extent = chooseSwapExtent(ctx, swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = ctx.surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // VK_SHARING_MODE_CONCURRENT will not be used only EXCLUSIVE will be used (one queue, and semaphor)
    //if (graphicsFamily != presentFamily) {
    //    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    //    createInfo.queueFamilyIndexCount = 2;
    //    createInfo.pQueueFamilyIndices = families;
    //}
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(ctx.device, &createInfo, nullptr, &ctx.swap_chain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(ctx.device, ctx.swap_chain, &imageCount, nullptr);
    ctx.swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(ctx.device, ctx.swap_chain, &imageCount, ctx.swapChainImages.data());

    ctx.swapChainImageFormat = surfaceFormat.format;
    ctx.swapChainExtent = extent;
}