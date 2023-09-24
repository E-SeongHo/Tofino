
#include <iostream>
#include <memory>
#include <directxtk/SimpleMath.h>

#include "Graphics.h"
#include "Model.h"

// using Microsoft::WRL::ComPtr;
using namespace std;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

Graphics::Graphics(HWND hWnd, const int screenWidth, const int screenHeight)
    :m_window(hWnd), m_width(screenWidth), m_height(screenHeight), m_screenViewport(D3D11_VIEWPORT())
{
}


//void Graphics::MakeTriangle()
//{
//    vector<Vertex> vertices;
//    vertices.push_back(Vertex{ Vector3{ -0.8f, -0.8f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });
//    vertices.push_back(Vertex{ Vector3{ 0.0f, 0.8f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });
//    vertices.push_back(Vertex{ Vector3{ 0.8f, -0.8f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });
//
//    vector<uint16_t> indices = { 0, 1, 2 };
//    triangleIndexCount = indices.size();
//    CreateVertexBuffer(vertices, triangleBuffer);
//    CreateIndexBuffer(indices, triangleIndexBuffer); 
//
//    triangleConst.model = Matrix();
//    triangleConst.view = Matrix();
//    triangleConst.projection = Matrix();
//
//    CreateConstantBuffer(triangleConst, triangleConstantBuffer);
//}

//void Graphics::CreateVertexBuffer(const std::vector<Vertex>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer) {
//
//    // D3D11_USAGE enumeration (d3d11.h)
//    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_usage
//    D3D11_BUFFER_DESC ds;
//    ZeroMemory(&ds, sizeof(ds));
//    ds.Usage = D3D11_USAGE_IMMUTABLE;
//    ds.ByteWidth = UINT((sizeof(Vertex)) * vertices.size());
//    ds.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//    ds.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
//    ds.StructureByteStride = sizeof(Vertex);
//
//    D3D11_SUBRESOURCE_DATA data = { 0 };
//    data.pSysMem = vertices.data();
//    data.SysMemPitch = 0;
//    data.SysMemSlicePitch = 0;
//
//    ThrowIfFailed(m_device->CreateBuffer(&ds, &data, vertexBuffer.GetAddressOf()));
//}
//
//void Graphics::CreateIndexBuffer(const std::vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
//{
//    D3D11_BUFFER_DESC ds = {};
//    // ZeroMemory(&ds, sizeof(ds));
//    ds.Usage = D3D11_USAGE_IMMUTABLE;
//    ds.ByteWidth = UINT(indices.size() * sizeof(uint16_t));
//    ds.BindFlags = D3D11_BIND_INDEX_BUFFER;
//    ds.CPUAccessFlags = 0;
//    ds.StructureByteStride = sizeof(uint16_t);
//
//    D3D11_SUBRESOURCE_DATA data = { 0 };
//    data.pSysMem = indices.data();
//    data.SysMemPitch = 0;
//    data.SysMemSlicePitch = 0;
//
//    ThrowIfFailed(m_device->CreateBuffer(&ds, &data, indexBuffer.GetAddressOf()));
//}
//
//void Graphics::CreateConstantBuffer(const MVPBuffer& cbufferData, ComPtr<ID3D11Buffer>& constantBuffer)
//{
//    D3D11_BUFFER_DESC cbDesc;
//    cbDesc.ByteWidth = sizeof(cbufferData);
//    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
//    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//    cbDesc.MiscFlags = 0;
//    cbDesc.StructureByteStride = 0;
//
//    // Fill in the subresource data.
//    D3D11_SUBRESOURCE_DATA InitData;
//    InitData.pSysMem = &cbufferData;
//    InitData.SysMemPitch = 0;
//    InitData.SysMemSlicePitch = 0;
//
//    ThrowIfFailed(m_device->CreateBuffer(&cbDesc, &InitData, constantBuffer.GetAddressOf()));
//}
bool Graphics::Init()
{
    InitD3D(m_width, m_height);
    
    model = make_shared<Triangle>();
    model->Init();
    model->CreateBuffers(m_device);

    InitShaders();

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

bool Graphics::InitShaders()
{
    ID3DBlob* vertexBlob = nullptr;
    ID3DBlob* pixelBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    if (FAILED(D3DCompileFromFile(L"BasicVS.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &vertexBlob, &errorBlob)))
    {
        if (errorBlob) {
            std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
        }
    }

    if (FAILED(D3DCompileFromFile(L"BasicPS.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &pixelBlob, &errorBlob)))
    {
        if (errorBlob) {
            std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
        }
    }

    m_device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &m_basicVertexShader);
    m_device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &m_basicPixelShader);

    // Create the input layout object
    vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    m_device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()), vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &m_basicInputLayout);

    return true;
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

    m_context->VSSetShader(m_basicVertexShader.Get(), 0, 0);

    m_context->PSSetShader(m_basicPixelShader.Get(), 0, 0);

    m_context->RSSetState(m_rasterizerState.Get());

    // Set Vertex & Index Buffer
    m_context->IASetInputLayout(m_basicInputLayout.Get());
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    model->Render(m_context);
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

// Note : CreateInputLayout requires the vertex shader in addition to the vertex input layout.
// because it cross checks the two to make sure the given vertex layout(parameter로 전달된) contains all the information 
// vertex shader asks for.
// https://www.walkerb.net/blog/dx-3/

