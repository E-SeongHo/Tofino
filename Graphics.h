#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <exception>
#include <vector>
#include <directxtk/SimpleMath.h>

#include "Model.h"
#include "Camera.h"
#include "ShaderManager.h"

using Microsoft::WRL::ComPtr;
class Graphics
{
private:
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

    std::shared_ptr<Camera> cam;
    std::shared_ptr<Geometry> model;


public:
    Graphics(HWND hWnd, const int screenWidth, const int screenHeight);
    // ~Graphics();
    
    bool Init();
    bool InitD3D(const int screenWidth, const int screenHeight);
    bool SetupGUIBackEnd();

    void Update(float dt);
    void Render();
    void Present();

    void UpdateGUI();
    float GetAspectRatio();
};