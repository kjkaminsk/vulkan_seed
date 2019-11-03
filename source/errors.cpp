#include <vulkan/vulkan.h>

#include <string>

#include "errors.h"

// make strings from vulkan result enums
#define S(r) case r: return #r
std::string get_result_name(VkResult result)
{
    switch (result)
    {
        S(VK_NOT_READY);
        S(VK_TIMEOUT);
        S(VK_EVENT_SET);
        S(VK_EVENT_RESET);
        S(VK_INCOMPLETE);
        S(VK_ERROR_OUT_OF_HOST_MEMORY);
        S(VK_ERROR_OUT_OF_DEVICE_MEMORY);
        S(VK_ERROR_INITIALIZATION_FAILED);
        S(VK_ERROR_DEVICE_LOST);
        S(VK_ERROR_MEMORY_MAP_FAILED);
        S(VK_ERROR_LAYER_NOT_PRESENT);
        S(VK_ERROR_EXTENSION_NOT_PRESENT);
        S(VK_ERROR_FEATURE_NOT_PRESENT);
        S(VK_ERROR_INCOMPATIBLE_DRIVER);
        S(VK_ERROR_TOO_MANY_OBJECTS);
        S(VK_ERROR_FORMAT_NOT_SUPPORTED);
        S(VK_ERROR_FRAGMENTED_POOL);
        S(VK_ERROR_OUT_OF_POOL_MEMORY);
        S(VK_ERROR_INVALID_EXTERNAL_HANDLE);
        S(VK_ERROR_SURFACE_LOST_KHR);
        S(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
        S(VK_SUBOPTIMAL_KHR);
        S(VK_ERROR_OUT_OF_DATE_KHR);
        S(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
        S(VK_ERROR_VALIDATION_FAILED_EXT);
        S(VK_ERROR_INVALID_SHADER_NV);
        S(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
        S(VK_ERROR_FRAGMENTATION_EXT);
        S(VK_ERROR_NOT_PERMITTED_EXT);
        S(VK_ERROR_INVALID_DEVICE_ADDRESS_EXT);
        S(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
    default:
        return std::string("VK_UNKNOWN_ERROR [") + std::to_string(result) + "]";
    }
}
#undef S

// Throw If Failed, a wrapper for checking vulkan api call result
// don't want to use macro (it kills intellisense for vulkan api calls) instead a line number and a file name are passed
// don't want to write 3 lines instead of 1 and providing not so usefull message.
// exceptions will break the app and allow for RAII if used
// an exception can be catched in main() or left unhandled, the point is not to contiue
#define FL __FILE__, __LINE__
void tif(char * file_line, int line, VkResult result)
{
    if (result != VK_SUCCESS) {
        auto result_string = get_result_name(result);
        throw std::runtime_error((result_string + " in file: \"" + file_line + "\" at line: " + std::to_string(line)).c_str());
    }
}
