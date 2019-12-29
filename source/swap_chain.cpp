#include <vulkan/vulkan.h>

#include <vector>
#include <algorithm>
#include <iostream>

#include "context.h"
#include "errors.h"
#include "swap_chain.h"

struct Swap_Chain_Support_Details {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

// query is used when physical devices are checked before choosing
// and after creating device (so always a physical_device handle is passed)
Swap_Chain_Support_Details query_swap_chain_support(Context& ctx, VkPhysicalDevice physical_device)
{
    Swap_Chain_Support_Details details;

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

// prefer BGRA NONLINEAR, if not found, take the first one
VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

// prefer MAILBOX, if not found blindly take FIFO as always supported
VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

// width capabilites == MAX means, don't care, so simply take what is needed
VkExtent2D choose_swap_extent(Context& ctx, const VkSurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }
    else {
        // initially ctx.width, ctx.height would be OK
        VkExtent2D actualExtent = { ctx.width, ctx.height };

        // but after resizing the window, we don't know the actual extent resolution
        //{
        //    int width, height;
        //    //glfwGetFramebufferSize(ctx.window, &width, &height);
        //    VkExtent2D actualExtent = { (uint32_t)width, (uint32_t)height };

        //    actualExtent.width = std::max(capabilities.minImageExtent.width,
        //                                  std::min(capabilities.maxImageExtent.width, actualExtent.width));
        //    actualExtent.height = std::max(capabilities.minImageExtent.height,
        //                                   std::min(capabilities.maxImageExtent.height, actualExtent.height));
        //}
        return actualExtent;
    }
}

void create_image_views(Context& ctx)
{
    ctx.swapChainImageViews.resize(ctx.image_count);

    VkImageViewCreateInfo createInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.layerCount = 1;

    for (size_t i = 0; i < ctx.swapChainImageViews.size(); i++) {
        createInfo.image = ctx.swapChainImages[i];
        createInfo.format = ctx.swap_chain_format;
        tif(FL, vkCreateImageView(ctx.device, &createInfo, nullptr, &ctx.swapChainImageViews[i]));
    }
}

// this check is used when physical devices are examined, so the context does not have physical_device chosen yet
bool is_swap_chain_acceptable(Context& ctx, VkPhysicalDevice physical_device)
{
    Swap_Chain_Support_Details swapChainSupport = query_swap_chain_support(ctx, physical_device);
    return !swapChainSupport.formats.empty() && !swapChainSupport.present_modes.empty();
}

void cleanup_swap_chain(Context& ctx)
{
    for (auto imageView : ctx.swapChainImageViews) {
        vkDestroyImageView(ctx.device, imageView, nullptr);
    }
    // images will be destroyed by swap chain destruction automatically
    vkDestroySwapchainKHR(ctx.device, ctx.swap_chain, nullptr);
}

void create_swap_chain(Context& ctx)
{
    Swap_Chain_Support_Details swapChainSupport = query_swap_chain_support(ctx, ctx.physical_device);

    VkSurfaceFormatKHR surfaceFormat = choose_swap_surface_format(swapChainSupport.formats);
    VkPresentModeKHR presentMode = choose_swap_present_mode(swapChainSupport.present_modes);
    VkExtent2D extent = choose_swap_extent(ctx, swapChainSupport.capabilities);

    if (extent.width != ctx.width || extent.height != ctx.height) {
        std::cout << "resolution changed: " << extent.width << "x" << extent.height <<std::endl;
        ctx.width = extent.width;
        ctx.height = extent.height;
    }

    ctx.image_count = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && ctx.image_count > swapChainSupport.capabilities.maxImageCount) {
        ctx.image_count = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = { VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR };
    createInfo.surface = ctx.surface;
    createInfo.minImageCount = ctx.image_count;
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

    // for window resize the old swap chain must be referenced
    // what if not?
    // createInfo.oldSwapchain = nullptr;

    tif(FL, vkCreateSwapchainKHR(ctx.device, &createInfo, nullptr, &ctx.swap_chain));

    vkGetSwapchainImagesKHR(ctx.device, ctx.swap_chain, &ctx.image_count, nullptr);
    ctx.swapChainImages.resize(ctx.image_count);
    vkGetSwapchainImagesKHR(ctx.device, ctx.swap_chain, &ctx.image_count, ctx.swapChainImages.data());

    ctx.swap_chain_format = surfaceFormat.format;

    //ctx.swapChainExtent = extent;

    create_image_views(ctx);
}
