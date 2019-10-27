#pragma once

typedef struct Context_ {
    int width;
    int height;

    GLFWwindow* window;

    VkInstance instance;
} Context;

