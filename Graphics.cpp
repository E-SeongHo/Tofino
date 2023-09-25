
#include <iostream>
#include <memory>
#include <directxtk/SimpleMath.h>
#include <imgui_impl_dx11.h>

#include "Graphics.h"
#include "Model.h"

// using Microsoft::WRL::ComPtr;
using namespace std;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

Graphics::Graphics(HWND hWnd, const int screenWidth, const int screenHeight)
    :m_window(hWnd), m_width(screenWidth), m_height(screenHeight), 
    m_screenViewport(D3D11_VIEWPORT()), cam(make_shared<Camera>())
{
    cam->SetAspect(this->GetAspectRatio());
    
    // Early Initialization of Singleton
    ShaderManager::GetInstance();
}

bool Graphics::Init()
{
    InitD3D(m_width, m_height);
    
    //model = make_shared<Triangle>();
    model = make_shared<Cube>();

    model->Init();
    model->CreateBuffers(m_device);

    ShaderManager::GetInstance().InitShaders(m_device);
    //InitShaders();

    model->UpdateViewMatrix(cam->GetViewMatrix().Transpose());
    model->UpdateProjectionMatrix(cam->GetProjectionMatrix().Transpose());

    model->UpdateBuffer(m_context); 

    return true;
}

bool Graphics::InitD3D(const int screenWidth, const int screenHeight)
{
    // Create SwapChain, d3d device, d3d device context
    const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // !temp - Float RTV 할 때 수정
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;

    const D3D_FEATURE_LEVEL featureLevels[2] = {
        D3D_FEATURE_LEVEL_11_0, // 더 높은 버전이 먼저 오도록 설정
        D3D_FEATURE_LEVEL_9_3 };
    D3D_FEATURE_LEVEL featureLevel;

    ThrowIfFailed(D3D11CreateDevice(
        nullptr,                  // Specify nullptr to use the default adapter.
        driverType,               // Create a device using the hardware graphics driver.
        0,                        // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
        createDeviceFlags,        // Set debug and Direct2D compatibility flags.
        featureLevels,            // List of feature levels this app can support.
        ARRAYSIZE(featureLevels), // Size of the list above.
        D3D11_SDK_VERSION,     // Always set this to D3D11_SDK_VERSION for Microsoft Store apps.
        device.GetAddressOf(), // Returns the Direct3D device created.
        &featureLevel,         // Returns feature level of device created.
        context.GetAddressOf() // Returns the device immediate context.
    ));

    UINT numQualityLevels;
    device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);

    if (FAILED(device.As(&m_device))) {
        cout << "device.AS() failed." << endl;
        return false;
    }

    if (FAILED(context.As(&m_context))) {
        cout << "context.As() failed." << endl;
        return false;
    } //  - temp!

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.Width = screenWidth;
    sd.BufferDesc.Height = screenHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferCount = 2;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = m_window;
    sd.Windowed = TRUE;
    sd.Flags =
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching
    // sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //ImGui 폰트가 두꺼워짐 
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    if (numQualityLevels > 0) {
        sd.SampleDesc.Count = 4; // how many multisamples
        sd.SampleDesc.Quality = numQualityLevels - 1;
    }
    else {
        sd.SampleDesc.Count = 1; // how many multisamples
        sd.SampleDesc.Quality = 0;
    }

    ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
        0, driverType, 0, createDeviceFlags, featureLevels, 1,
        D3D11_SDK_VERSION, &sd, m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(), &featureLevel, m_context.GetAddressOf()));

    if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
        cout << "D3D Feature Level 11 unsupported." << endl;
        return false;
    }
   
    // Create RenderTargetView
    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    ThrowIfFailed(m_device->CreateRenderTargetView(backBuffer.Get(), NULL, m_renderTargetView.GetAddressOf()));

    // Set the viewport
    ZeroMemory(&m_screenViewport, sizeof(D3D11_VIEWPORT));
    m_screenViewport.TopLeftX = 0;
    m_screenViewport.TopLeftY = 0;
    m_screenViewport.Width = float(screenWidth);
    m_screenViewport.Height = float(screenHeight);
    // m_screenViewport.Width = static_cast<float>(m_screenHeight);
    m_screenViewport.MinDepth = 0.0f;
    m_screenViewport.MaxDepth = 1.0f; // Note: important for depth buffering

    m_context->RSSetViewports(1, &m_screenViewport);

    // Create a rasterizer state
    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC)); // Need this
    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    // rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
    rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    rastDesc.FrontCounterClockwise = false;
    rastDesc.DepthClipEnable = true; // <- zNear, zFar 확인에 필요

    m_device->CreateRasterizerState(&rastDesc, m_rasterizerState.GetAddressOf());


    // Create depth buffer
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    depthStencilBufferDesc.Width = screenWidth;
    depthStencilBufferDesc.Height = screenHeight;
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    if (numQualityLevels > 0) {
        depthStencilBufferDesc.SampleDesc.Count = 4; // how many multisamples
        depthStencilBufferDesc.SampleDesc.Quality = numQualityLevels - 1;
    }
    else {
        depthStencilBufferDesc.SampleDesc.Count = 1; // how many multisamples
        depthStencilBufferDesc.SampleDesc.Quality = 0;
    }
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0;
    depthStencilBufferDesc.MiscFlags = 0;

    ThrowIfFailed(m_device->CreateTexture2D(&depthStencilBufferDesc, 0,
        m_depthStencilBuffer.GetAddressOf()));

    ThrowIfFailed(m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, &m_depthStencilView));

    // Create Depth Stencil State
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = true; 
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
    
    ThrowIfFailed(m_device->CreateDepthStencilState(&depthStencilDesc,
        m_depthStencilState.GetAddressOf()));

    return true;
}

bool Graphics::SetupGUIBackEnd()
{
    if (!ImGui_ImplDX11_Init(m_device.Get(), m_context.Get())) 
        return false;
}

void Graphics::Update(float dt)
{
    model->UpdateWorldMatrix(model->GetWorldMatrix() * Matrix::CreateRotationZ(1.0f * dt).Transpose());
    model->UpdateBuffer(m_context);
}

void Graphics::Render()
{
    m_context->RSSetViewports(1, &m_screenViewport);

    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f, 0);

    m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
    m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

    m_context->VSSetShader(ShaderManager::GetInstance().basicVS.Get(), 0, 0);

    m_context->PSSetShader(ShaderManager::GetInstance().basicPS.Get(), 0, 0);

    m_context->RSSetState(m_rasterizerState.Get());

    // Set Vertex & Index Buffer
    m_context->IASetInputLayout(ShaderManager::GetInstance().basicInputLayout.Get());
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    model->Render(m_context);
}

void Graphics::Present()
{
    m_swapChain->Present(1, 0);
}

void Graphics::UpdateGUI()
{

}


float Graphics::GetAspectRatio()
{
    return float(m_width) / m_height;
}

// How To: Compile a Shader
// https://docs.microsoft.com/en-us/windows/win32/direct3d11/how-to--compile-a-shader

void CheckResult(HRESULT hr, ID3DBlob* errorBlob) {
    if (FAILED(hr)) {
        // 파일이 없을 경우
        if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0) {
            cout << "File not found." << endl;
        }

        // 에러 메시지가 있으면 출력
        if (errorBlob) {
            cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << endl;
        }
    }
}



