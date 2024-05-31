//
// Created by lany on 24-5-6.
//

#include "gui.h"
#include "config/config.h"

#include <atomic>

#include "imgui.h"
#ifdef IMGUI_USE_DX11
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#endif
#ifdef IMGUI_USE_WIN32
#include "imgui_impl_win32.h"
#endif

#ifdef IMGUI_USE_DX11
static ID3D11Device *g_pd3dDevice = nullptr;
static ID3D11DeviceContext *g_pd3dDeviceContext = nullptr;
static IDXGISwapChain *g_pSwapChain = nullptr;
static ID3D11RenderTargetView *g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

bool CreateDeviceD3D(HWND hWnd) {
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    // createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };
    HRESULT res = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain,
        &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software
                                       // driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags,
            featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain,
            &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (g_pSwapChain) {
        g_pSwapChain->Release();
        g_pSwapChain = nullptr;
    }
    if (g_pd3dDeviceContext) {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = nullptr;
    }
    if (g_pd3dDevice) {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }
}

void CreateRenderTarget() {
    ID3D11Texture2D *pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr,
                                         &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget() {
    if (g_mainRenderTargetView) {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = nullptr;
    }
}

bool plat_init_backend_dx11(void *p) {
    HWND hwnd = (HWND)p;
    if (!CreateDeviceD3D(hwnd)) {
        CleanupDeviceD3D();
        return false;
    }
    return ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
}
void plat_clean_backend_dx11() {
    ImGui_ImplDX11_Shutdown();
    CleanupDeviceD3D();
}
void plat_resize_backend_dx11(const UINT w, const UINT h) {
    if (w != 0 && h != 0) {
        CleanupRenderTarget();
        g_pSwapChain->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);
        CreateRenderTarget();
    }
}
void plat_frame_begin_backend_dx11() { ImGui_ImplDX11_NewFrame(); }
void plat_frame_render_backend_dx11() {
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    ImGui::Render();
    const float clear_color_with_alpha[4] = {
        clear_color.x * clear_color.w, clear_color.y * clear_color.w,
        clear_color.z * clear_color.w, clear_color.w};
    g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView,
                                            nullptr);
    g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView,
                                               clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void plat_frame_end_backend_dx11() {
    g_pSwapChain->Present(1, 0); // Present with vsync
}

#endif // IMGUI_USE_DX11

bool plat_init_backend(void *p) {
#ifdef IMGUI_USE_DX11
    return plat_init_backend_dx11(p);
#endif
}
void plat_clean_backend() {
#ifdef IMGUI_USE_DX11
    plat_clean_backend_dx11();
#endif
}
void plat_resize_backend(const int w, const int h) {
#ifdef IMGUI_USE_DX11
    plat_resize_backend_dx11(w, h);
#endif
}

void plat_frame_begin_backend() {
#ifdef IMGUI_USE_DX11
    plat_frame_begin_backend_dx11();
#endif
}
void plat_frame_render_backend() {
#ifdef IMGUI_USE_DX11
    plat_frame_render_backend_dx11();
#endif
}

void plat_frame_end_backend() {
#ifdef IMGUI_USE_DX11
    plat_frame_end_backend_dx11();
#endif
}

#ifdef IMGUI_USE_WIN32
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_MOVE:
        set_window_pos(LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        plat_resize_backend(LOWORD(lParam), HIWORD(lParam));
        set_window_size(LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

static HWND plat_hwnd = nullptr;
static WNDCLASSEXW plat_wc = {0};
bool plat_init_win32(int x, int y, int width, int height, bool fullscreen) {

    HINSTANCE hInstance = GetModuleHandle(nullptr);
    // HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(1));
    WNDCLASSEXW wc = {
        sizeof(wc), CS_CLASSDC, WndProc, 0L,      0L,       hInstance,
        nullptr,    nullptr,    nullptr, nullptr, L"blive", nullptr,
    };
    RegisterClassExW(&wc);
    plat_wc = wc;
    HWND hwnd =
        CreateWindowW(wc.lpszClassName, L"blive", WS_OVERLAPPEDWINDOW, x, y,
                      width, height, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!plat_init_backend((void *)hwnd)) {
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return false;
    }

    ImGui_ImplWin32_Init(hwnd);

    // Show the window
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    plat_hwnd = hwnd;
    return true;
}

void plat_clean_win32() {
    plat_clean_backend();
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(plat_hwnd);
    UnregisterClassW(plat_wc.lpszClassName, plat_wc.hInstance);
}

void plat_frame_before_win32() {
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            done.store(true, std::memory_order_release);
    }
}

void plat_frame_begin_win32() {
    plat_frame_begin_backend();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void plat_frame_end_win32() { plat_frame_end_backend(); }

#endif // IMGUI_USE_WIN32

bool gui_init(int x, int y, int width, int height, bool fullscreen) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
#ifdef IMGUI_USE_WIN32
    return plat_init_win32(x, y, width, height, fullscreen);
#endif
    return false;
}

void gui_clean() {
#ifdef IMGUI_USE_WIN32
    plat_clean_win32();
#endif
    ImGui::DestroyContext();
}

void plat_frame_before() {
#ifdef IMGUI_USE_WIN32
    plat_frame_before_win32();
#endif
}

void plat_frame_begin() {
#ifdef IMGUI_USE_WIN32
    plat_frame_begin_win32();
#endif
}

void plat_frame_render() { plat_frame_render_backend(); }

void plat_frame_end() {
#ifdef IMGUI_USE_WIN32
    plat_frame_end_win32();
#endif
}

void gui_frame(frame_func func) {
    plat_frame_before();
    plat_frame_begin();
    func();
    plat_frame_render();
    plat_frame_end();
}