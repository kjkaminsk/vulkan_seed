#include <vulkan/vulkan.h>

#include <string>

#include "errors.h"

// make strings from vulkan result enums
#define S(r) case VK_ ##r: name += #r
std::string get_result_name(VkResult result)
{
    std::string name = "VK_";
    switch (result)
    {
        S(NOT_READY);
        S(TIMEOUT);
        S(EVENT_SET);
        S(EVENT_RESET);
        S(INCOMPLETE);
        S(ERROR_OUT_OF_HOST_MEMORY);
        S(ERROR_OUT_OF_DEVICE_MEMORY);
        S(ERROR_INITIALIZATION_FAILED);
        S(ERROR_DEVICE_LOST);
        S(ERROR_MEMORY_MAP_FAILED);
        S(ERROR_LAYER_NOT_PRESENT);
        S(ERROR_EXTENSION_NOT_PRESENT);
        S(ERROR_FEATURE_NOT_PRESENT);
        S(ERROR_INCOMPATIBLE_DRIVER);
        S(ERROR_TOO_MANY_OBJECTS);
        S(ERROR_FORMAT_NOT_SUPPORTED);
        S(ERROR_FRAGMENTED_POOL);
        S(ERROR_OUT_OF_POOL_MEMORY);
        S(ERROR_INVALID_EXTERNAL_HANDLE);
        S(ERROR_SURFACE_LOST_KHR);
        S(ERROR_NATIVE_WINDOW_IN_USE_KHR);
        S(SUBOPTIMAL_KHR);
        S(ERROR_OUT_OF_DATE_KHR);
        S(ERROR_INCOMPATIBLE_DISPLAY_KHR);
        S(ERROR_VALIDATION_FAILED_EXT);
        S(ERROR_INVALID_SHADER_NV);
        S(ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
        S(ERROR_FRAGMENTATION_EXT);
        S(ERROR_NOT_PERMITTED_EXT);
        S(ERROR_INVALID_DEVICE_ADDRESS_EXT);
        S(ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
    default:
        name += "UNKNOWN_ERROR [" + std::to_string(result) + "]";
    }
    return name;
}
#undef STR

// Throw If Failed, a wrapper for checking vulkan api call result
// don't want to use macro (it kills intellisense for vulkan api calls) instead a line number and a file name are passed
// don't want to write 3 lines instead of 1 and providing not so usefull message.
// exceptions will break the app and allow for RAII if used
// an exception can be catched in main() or left unhandled
#define FL __FILE__, __LINE__
void tif(char * file_line, int line, VkResult result)
{
    if (result != VK_SUCCESS) {
        auto result_string = get_result_name(result);
        throw std::runtime_error((result_string + " in file: \"" + file_line + "\" at line: " + std::to_string(line)).c_str());
    }
}
