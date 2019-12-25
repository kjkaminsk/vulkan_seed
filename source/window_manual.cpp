#include <vulkan/vulkan.h>

#include "context.h"
#include "graphics_pass.h"
#include "errors.h"
#include "draw_frame.h"

void handle_window_messages(Context* ctx, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static Context* context = nullptr;
    if (ctx)
    {
        context = ctx;
        return;
    }

    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
        }
        break;
    }
}

static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    handle_window_messages(nullptr, hwnd, msg, wParam, lParam);

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void create_window_manual(Context& ctx)
{
    const std::string title = "Vulkan_Seed";

    const CHAR* window_class_name = title.c_str();

    // initialize context pointer in the handler
    handle_window_messages(&ctx, 0, 0, 0, 0);

    // Register the window class
    WNDCLASS window_class = {};
    window_class.lpfnWndProc = wnd_proc;
    window_class.hInstance = GetModuleHandle(nullptr);
    window_class.lpszClassName = window_class_name;

    if (RegisterClass(&window_class) == 0)
    {
        throw std::runtime_error("Window class registering failed.");
    }

    // Window size we have is for client area, calculate actual window size
    DWORD window_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    RECT r{ 0, 0, (LONG)ctx.width, (LONG)ctx.height };
    AdjustWindowRect(&r, window_style, false);

    int window_width = r.right - r.left;
    int window_height = r.bottom - r.top;

    // Create the window
    ctx.window_manual = CreateWindowEx(0, window_class_name, title.c_str(), window_style, CW_USEDEFAULT, CW_USEDEFAULT,
                                     window_width, window_height, nullptr, nullptr, window_class.hInstance, nullptr);
    if (ctx.window_manual == nullptr)
    {
        throw std::runtime_error("CreateWindowEx() failed");
    }

    // Show the window
    ShowWindow(ctx.window_manual, SW_SHOWNORMAL);
}

void cleanup_window_manual(Context& ctx)
{
    DestroyWindow(ctx.window_manual);
}

void main_loop_manual(Context& ctx, Graphics_Pass& pass)
{
    MSG msg;
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        draw_frame(ctx, pass);
    }
    vkDeviceWaitIdle(ctx.device);
}
