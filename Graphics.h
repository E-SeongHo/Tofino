#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <exception>
#include <vector>
#include <directxtk/SimpleMath.h>


inline void ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr)) {
        // Set a breakpoint on this line to catch Win32 API errors.
        throw std::exception();
    }
}

struct Vertex { // Match with inputelement
    DirectX::SimpleMath::Vector3 pos;
    DirectX::SimpleMath::Vector3 color;
};

struct ConstantBuffer {
    DirectX::SimpleMath::Matrix model;
    DirectX::SimpleMath::Matrix view;
    DirectX::SimpleMath::Matrix projection;
};

using Microsoft::WRL::ComPtr;
class Graphics
{
public:
    HWND m_window;
    int m_width, m_height;

    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11RasterizerState> m_rasterizerState;

    ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    ComPtr<ID3D11DepthStencilState> m_depthStencilState;

    D3D11_VIEWPORT m_screenViewport;

    // Shaders
    ComPtr<ID3D11VertexShader> m_basicVertexShader;
    ComPtr<ID3D11PixelShader> m_basicPixelShader;
    ComPtr<ID3D11InputLayout> m_basicInputLayout;

    // temp
    ComPtr<ID3D11Buffer> triangleBuffer;
    ComPtr<ID3D11Buffer> triangleIndexBuffer;
    ComPtr<ID3D11Buffer> triangleConstantBuffer;
    UINT triangleIndexCount;

    ConstantBuffer triangleConst;

public:
    Graphics(HWND hWnd, const int screenWidth, const int screenHeight);
    // ~Graphics();
    
    void MakeTriangle();

    bool Init();
    bool InitD3D(const int screenWidth, const int screenHeight);
    bool InitShaders();
    void CreateVertexBuffer(const std::vector<Vertex>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer);
    void CreateIndexBuffer(const std::vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer);
    void CreateConstantBuffer(const ConstantBuffer& cbufferData, ComPtr<ID3D11Buffer>& constantBuffer);

    void Render();

    bool CreateVertexShader();
};