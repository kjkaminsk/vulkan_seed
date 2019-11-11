cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(NAME vulkan_seed)
project(${NAME})

include_directories(external/glm)
include_directories(external/glfw3/include)

find_package(Vulkan)
IF (NOT Vulkan_FOUND)
  message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
  message(STATUS ${Vulkan_LIBRARY})
ENDIF()

find_library(GLFW3_LIBRARIES NAMES glfw3 glfw3.lib PATHS ${CMAKE_SOURCE_DIR}/external/glfw3/lib-vc2019)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -D_USE_MATH_DEFINES")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-std=c++11)

file(GLOB SOURCE ${CMAKE_SOURCE_DIR}/source/*.cpp)
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/source/*.h)

add_executable(${NAME} ${SOURCE} ${HEADERS}) # add WIN32 for window app
target_link_libraries(${NAME} Vulkan::Vulkan ${GLFW3_LIBRARIES} ${WINLIBS})
