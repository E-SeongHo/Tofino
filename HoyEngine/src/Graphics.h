#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <windows.h>
#include <exception>
#include <vector>
#include <directxtk/SimpleMath.h>

#include "Geometry.h"
#include "Model.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "Light.h"
#include "EnvMap.h"
#include "Scene.h"

using Microsoft::WRL::ComPtr;
class Graphics
{
public:
    static Graphics& GetInstance()
    {
        static Graphics s;
        return s;
    }
    ~Graphics();
       
    static bool Init(HWND hWnd, const int screenWidth, const int screenHeight);
    static bool InitD3D(const int screenWidth, const int screenHeight);
    static bool SetupGUIBackEnd();

    static void RenderScene(Scene* scene);
    static void Present();

    static ComPtr<ID3D11Device>& GetDevice();
    static ComPtr<ID3D11DeviceContext>& GetDeviceContext();

    // thinking of..
    void AddPSO();
    void SetDefaultPSO();
    void SetEnvPSO();
    void SetCopyPSO();
    void SetShadowPSO();

private:
    Graphics() = default;
    Graphics(const Graphics&) = delete;
    Graphics& operator=(const Graphics&) = delete;

    // Implementations of static functions
    bool _Init(HWND hWnd, const int screenWidth, const int screenHeight);
    bool _InitD3D(const int screenWidth, const int screenHeight);

    void _RenderScene(Scene* scene);

private:
    HWND m_window;
    int m_width, m_height;

    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11Texture2D> m_backBuffer; // final swapchain rendering buffer DXGI_FORMAT_R8G8B8A8_UNORM
    ComPtr<ID3D11RenderTargetView> m_backBufferRTV; // final RTV
    ComPtr<ID3D11RasterizerState> m_solidState;
    ComPtr<ID3D11RasterizerState> m_wireState;
    
    ComPtr<ID3D11SamplerState> m_samplerState;
    ComPtr<ID3D11SamplerState> m_clampSampler;

    ComPtr<ID3D11Texture2D> m_hdrBuffer; // use MSAA, DXGI_FORMAT_R16G16B16A16_FLOAT
    ComPtr<ID3D11Texture2D> m_hdrResolvedBuffer; // no MSAA, DXGI_FORMAT_R16G16B16A16_FLOAT

    ComPtr<ID3D11RenderTargetView> m_hdrRTV;
    ComPtr<ID3D11RenderTargetView> m_hdrResolvedRTV;

    ComPtr<ID3D11ShaderResourceView> m_hdrSRV;
    ComPtr<ID3D11ShaderResourceView> m_hdrResolvedSRV;
    
    ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    ComPtr<ID3D11DepthStencilState> m_depthStencilState;

    D3D11_VIEWPORT m_screenViewport;

    // Tone Mapping
    Geometry* m_copySquare = nullptr; // square mesh for copy
    ComPtr<ID3D11RasterizerState> m_toneState;

    bool m_wireRendering = false;
};