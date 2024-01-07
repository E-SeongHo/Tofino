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

struct GlobalBuffer
{   // Make sure must be a column matrix
    DirectX::SimpleMath::Matrix view;
    DirectX::SimpleMath::Matrix projection;
};

using Microsoft::WRL::ComPtr;
class Graphics
{
private:
    HWND m_window;
    int m_width, m_height;
    POINT m_aimPoint;

    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11Texture2D> m_backBuffer; // final swapchain rendering buffer DXGI_FORMAT_R8G8B8A8_UNORM
    ComPtr<ID3D11RenderTargetView> m_backBufferRTV; // final RTV
    ComPtr<ID3D11RasterizerState> m_solidState;
    ComPtr<ID3D11RasterizerState> m_wireState;

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

    Camera* cam = nullptr;
    Geometry* sphere = nullptr;
    Model* model = nullptr;
    EnvMap* envMap = nullptr;
    Geometry* pickingEffect = nullptr;

    GlobalBuffer m_globalConstBufferCPU;
    ComPtr<ID3D11Buffer> m_globalConstBufferGPU;

    // Tone Mapping
    Geometry* m_copySquare = nullptr; // square mesh for copy
    ComPtr<ID3D11SamplerState> m_samplerState;
    ComPtr<ID3D11RasterizerState> m_toneState;

    bool m_fpvMode = false;
public:
    Graphics(HWND hWnd, const int screenWidth, const int screenHeight);
    ~Graphics();
    
    bool Init();
    bool InitD3D(const int screenWidth, const int screenHeight);
    bool SetupGUIBackEnd();
    void SetGlobalConstantBuffers();

    void Update(float dt);
    void Render();
    void Present();

    void UpdateGUI();
    float GetAspectRatio();
    void ToggleFPVMode();

    void ToneMappingSetUp();

    // Input Process
    void ProcessMouseMove(const int xPos, const int yPos);
    void ProcessMouseWheel(const int wheel);
    void UpdateCameraPosition(float dt);

public:
    bool m_keyState[256] = { false, }; // TODO: InputController class·Î ºÐ¸®
    bool m_leftClick = false;
    bool m_rightClick = false;
    bool m_picking = false;
    DirectX::SimpleMath::Plane m_draggingPlane;
    DirectX::SimpleMath::Vector3 prevHit;
};