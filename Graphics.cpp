
#include <iostream>
#include <memory>
#include <directxtk/SimpleMath.h>
#include <imgui_impl_dx11.h>
#include <DirectXCollision.h>

#include "Graphics.h"
#include "Geometry.h"

// using Microsoft::WRL::ComPtr;
using namespace std;
using DirectX::BoundingSphere;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Ray;
using DirectX::SimpleMath::Plane;

Graphics::Graphics(HWND hWnd, const int screenWidth, const int screenHeight)
    :m_window(hWnd), m_width(screenWidth), m_height(screenHeight), 
    m_aimPoint({m_width/2, m_height/2}), m_screenViewport(D3D11_VIEWPORT())
{
    // Early Initialization of Singleton
    ShaderManager::GetInstance();
}

bool Graphics::Init()
{
    InitD3D(m_width, m_height);
    ClientToScreen(m_window, &m_aimPoint);
    
    cam = new Camera();
    cam->SetAspect(this->GetAspectRatio());

    //model = make_shared<Triangle>();
    //model = new Cube();
    sphere = new Sphere();
    sphere->LoadGeometry();

    const string texDirectory = "./Assets/Texture/wet-mossy-rocks-ue/";
    sphere->AttachAlbedoTexture(texDirectory + "wet-mossy-rocks_albedo.png");
    sphere->AttachNormalTexture(texDirectory + "wet-mossy-rocks_normal-dx.png");
    sphere->AttachHeightTexture(texDirectory + "wet-mossy-rocks_height.png");
    sphere->SetMaterials(0.5f, 0.1f);
    sphere->Init(m_device, true);

    spaceship = new Model();
    spaceship->LoadModel("D:/Workspace/3DModels/sci-fi-space-station/SpaceStation.fbx", 10.0f);
    spaceship->AttachAlbedoTexture(spaceship->m_directory + "SpaceStationParts2_BaseColor.png", 0);
    spaceship->Init(m_device, true);
    objects.push_back(spaceship);

    aircraft = new Model();
    aircraft->LoadModel("D:/Workspace/3DModels/E-45-Aircraft/E 45 Aircraft_obj.obj", 3.0f);
    aircraft->AttachAlbedoTexture(aircraft->m_directory + "E-45 _col.jpg", 0);
    aircraft->AttachNormalTexture(aircraft->m_directory + "E-45-nor_1.jpg", 0);
    aircraft->AttachHeightTexture(aircraft->m_directory + "E-45-nor_1.jpg", 0);
    aircraft->AttachAlbedoTexture("", 1);
    aircraft->AttachNormalTexture(aircraft->m_directory + "E-45_glass_nor_.jpg", 1);
    aircraft->AttachHeightTexture(aircraft->m_directory + "E-45_glass_nor_.jpg", 1);
    aircraft->SetMaterials(0.1f, 0.5f);
    aircraft->Init(m_device, true);
    objects.push_back(aircraft);

    pickingEffect = new Sphere();
    pickingEffect->LoadGeometry(0.1f);
    pickingEffect->SetMaterials(0.1f, 0.3f, Vector3(0.0f, 0.4f, 0.4f));
    pickingEffect->Init(m_device, false);

    m_copySquare = new Square();
    m_copySquare->LoadGeometry();
    m_copySquare->Init(m_device, false);

    envMap = new EnvMap();
    envMap->Init(m_device, L"./Assets/Cubemap/HDRI/PlanetaryEarth/");

    Light light;
    light.pos = Vector3(-0.3f, 0.2f, -2.0f);
    light.strength = 0.5f;
    light.direction = Vector3(0.1f, -0.2f, 1.0f);
    light.coefficient = 2.2f;

    ShaderManager::GetInstance().InitShaders(m_device);

    m_globalConstBufferCPU.view = cam->GetViewMatrix().Transpose();
    m_globalConstBufferCPU.projection = cam->GetProjectionMatrix().Transpose();
    m_globalConstBufferCPU.eye = cam->GetOrigin();
    m_globalConstBufferCPU.light = light;

    Util::CreateConstantBuffer(m_device, m_globalConstBufferCPU, m_globalConstBufferGPU);

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

    const D3D_FEATURE_LEVEL featureLevels[2] = {
        D3D_FEATURE_LEVEL_11_0, // 더 높은 버전이 먼저 오도록 설정
        D3D_FEATURE_LEVEL_9_3 };
    D3D_FEATURE_LEVEL featureLevel;

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.Width = screenWidth;
    sd.BufferDesc.Height = screenHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // since usual monitor's format
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
    sd.SampleDesc.Count = 1; // _FLIP_은 MSAA 미지원
    sd.SampleDesc.Quality = 0;

    ThrowIfFailed(D3D11CreateDeviceAndSwapChain(
        0, driverType, 0, createDeviceFlags, featureLevels, 1,
        D3D11_SDK_VERSION, &sd, m_swapChain.GetAddressOf(),
        m_device.GetAddressOf(), &featureLevel, m_context.GetAddressOf()));

    if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
        cout << "D3D Feature Level 11 unsupported." << endl;
        return false;
    }
   
    // Create backbuffer RenderTargetView
    //ComPtr<ID3D11Texture2D> backBuffer; // m_backBuffer로 관리

    ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_backBuffer.GetAddressOf())));
    ThrowIfFailed(m_device->CreateRenderTargetView(m_backBuffer.Get(), NULL, m_backBufferRTV.GetAddressOf()));


    // Create HDR Buffer
    UINT numQualityLevels;
    ThrowIfFailed(m_device->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R16G16B16A16_FLOAT, 4, &numQualityLevels));

    D3D11_TEXTURE2D_DESC desc;
    m_backBuffer->GetDesc(&desc);
    desc.MipLevels = desc.ArraySize = 1;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    desc.Usage = D3D11_USAGE_DEFAULT; // 스테이징 텍스춰로부터 복사 가능
    desc.MiscFlags = 0;
    desc.CPUAccessFlags = 0;
    if (numQualityLevels) {
        desc.SampleDesc.Count = 4;
        desc.SampleDesc.Quality = numQualityLevels - 1;
    }
    else {
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
    }

    ThrowIfFailed(m_device->CreateTexture2D(&desc, NULL, m_hdrBuffer.GetAddressOf()));
    ThrowIfFailed(m_device->CreateShaderResourceView(m_hdrBuffer.Get(), NULL, m_hdrSRV.GetAddressOf()));
    ThrowIfFailed(m_device->CreateRenderTargetView(m_hdrBuffer.Get(), NULL, m_hdrRTV.GetAddressOf()));

    D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
    m_hdrRTV->GetDesc(&viewDesc);

    // HDR Resolved Buffer
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    ThrowIfFailed(m_device->CreateTexture2D(&desc, NULL, m_hdrResolvedBuffer.GetAddressOf()));
    ThrowIfFailed(m_device->CreateShaderResourceView(m_hdrResolvedBuffer.Get(), NULL, m_hdrResolvedSRV.GetAddressOf()));
    ThrowIfFailed(m_device->CreateRenderTargetView(m_hdrResolvedBuffer.Get(), NULL, m_hdrResolvedRTV.GetAddressOf()));

    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    m_device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());

    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    m_device->CreateSamplerState(&sampDesc, m_clampSampler.GetAddressOf());

    // Tone mapping!!!
    ToneMappingSetUp();

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
    // Make Both(solid, wire)
    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    rastDesc.FrontCounterClockwise = false;
    rastDesc.DepthClipEnable = true; // <- zNear, zFar 확인에 필요

    ThrowIfFailed(m_device->CreateRasterizerState(&rastDesc, m_solidState.GetAddressOf()));

    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

    ThrowIfFailed(m_device->CreateRasterizerState(&rastDesc, m_wireState.GetAddressOf()));

    // Create depth buffer :: m_hdrBuffer use this buffer
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

    ThrowIfFailed(m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, m_depthStencilView.GetAddressOf()));

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
    return true;
}

void Graphics::SetGlobalConstantBuffers()
{
    // register b10 will receive it
    m_context->VSSetConstantBuffers(10, 1, m_globalConstBufferGPU.GetAddressOf());
    m_context->GSSetConstantBuffers(10, 1, m_globalConstBufferGPU.GetAddressOf());
    m_context->PSSetConstantBuffers(10, 1, m_globalConstBufferGPU.GetAddressOf());
}

void Graphics::Update(float dt)
{
    // Camera
    UpdateCameraPosition(dt);
    m_globalConstBufferCPU.view = cam->GetViewMatrix().Transpose();
    m_globalConstBufferCPU.projection = cam->GetProjectionMatrix().Transpose();

    Util::UpdateConstantBuffer(m_context, m_globalConstBufferCPU, m_globalConstBufferGPU);

    // Model 
    //cout << model->m_boundingSphere.Center.z << endl;
    /*sphere->UpdateWorldMatrix(Matrix::CreateRotationY(1.0f * dt) * Matrix::CreateRotationX(1.0f * dt) * sphere->GetWorldMatrix());
    sphere->UpdateBuffer(m_context);*/
    
    sphere->UpdateBuffer(m_context);

    spaceship->UpdateWorldMatrix(Matrix::CreateRotationZ(0.5f * dt) * spaceship->GetWorldMatrix());
    spaceship->UpdateBuffer(m_context);

    aircraft->UpdateBuffer(m_context);
}

void Graphics::Render()
{
    //cout << cam->GetOrigin().x << " " << cam->GetOrigin().y << " " << cam->GetOrigin().z << endl;
    m_context->RSSetViewports(1, &m_screenViewport);

    const float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_context->ClearRenderTargetView(m_hdrRTV.Get(), clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f, 0);

    m_context->OMSetRenderTargets(1, m_hdrRTV.GetAddressOf(), m_depthStencilView.Get());
    m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

    m_context->VSSetShader(ShaderManager::GetInstance().basicVS.Get(), 0, 0);
    m_context->PSSetShader(ShaderManager::GetInstance().basicPS.Get(), 0, 0);

    ID3D11SamplerState* samplers[] = { m_samplerState.Get(), m_clampSampler.Get() };
    m_context->VSSetSamplers(0, 2, samplers);
    m_context->PSSetSamplers(0, 2, samplers);

    if (m_wireRendering)
    {
        m_context->RSSetState(m_wireState.Get());
    }
    else
    {
        m_context->RSSetState(m_solidState.Get());
    }

    // Set Vertex & Index Buffer
    m_context->IASetInputLayout(ShaderManager::GetInstance().basicInputLayout.Get());
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    SetGlobalConstantBuffers();
    envMap->SetIBLSRVs(m_context);

    //sphere->SetSRVs(m_context);
    //sphere->Render(m_context);
    
    for (auto& object : objects)
    {
        object->Render(m_context);
    }
    
    if (m_picking && m_leftClick)
    {
        pickingEffect->Render(m_context);
    }

    m_context->IASetInputLayout(ShaderManager::GetInstance().basicInputLayout.Get());
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

    m_context->VSSetShader(ShaderManager::GetInstance().normalVS.Get(), 0, 0);
    m_context->GSSetShader(ShaderManager::GetInstance().normalGS.Get(), 0, 0);
    m_context->PSSetShader(ShaderManager::GetInstance().normalPS.Get(), 0, 0);

    //model->RenderNormal(m_context);
    m_context->GSSetShader(nullptr, 0, 0);

    // cube map
    m_context->IASetInputLayout(ShaderManager::GetInstance().basicInputLayout.Get());
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_context->VSSetShader(ShaderManager::GetInstance().envMapVS.Get(), 0, 0);
    m_context->PSSetShader(ShaderManager::GetInstance().envMapPS.Get(), 0, 0);
    m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
    
    SetGlobalConstantBuffers();
    envMap->Render(m_context);

    // Resolve
    m_context->ResolveSubresource(m_hdrResolvedBuffer.Get(), 0, m_hdrBuffer.Get(), 
        0, DXGI_FORMAT_R16G16B16A16_FLOAT);

    // Tone mapping ( just simply gamma correction )
    //m_context->RSSetViewports(1, &m_screenViewport);
    m_context->ClearRenderTargetView(m_backBufferRTV.Get(), clearColor);
    m_context->RSSetState(m_toneState.Get());

    m_context->IASetInputLayout(ShaderManager::GetInstance().basicInputLayout.Get()); // use same as basic
    m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // use same as basic
    m_context->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), NULL);
    
    m_context->VSSetShader(ShaderManager::GetInstance().copyVS.Get(), 0, 0);
    m_context->PSSetSamplers(0, 1, m_clampSampler.GetAddressOf());
    m_context->PSSetShader(ShaderManager::GetInstance().toneMappingPS.Get(), 0, 0);
    m_context->PSSetShaderResources(0, 1, m_hdrResolvedSRV.GetAddressOf());

    m_copySquare->m_meshes[0].Render(m_context);
}

void Graphics::Present()
{
    m_swapChain->Present(1, 0);
}

void Graphics::RenderGUI()
{
    ImGui::StyleColorsClassic();
    ImGui::NewFrame(); 
    ImGui::Begin("Scene Control");

    ImGui::Text("Avg: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
        ImGui::GetIO().Framerate);

    if (ImGui::CollapsingHeader("Rendering Options"))
    {
        ImGui::Checkbox("Show WireFrame", &m_wireRendering);
    }
    
    if (ImGui::CollapsingHeader("Light"))
    {
        ImGui::SliderFloat3("Origin", &m_globalConstBufferCPU.light.pos.x, -200.0f, 200.0f);
        ImGui::SliderFloat("Strength", &m_globalConstBufferCPU.light.strength, 0.0f, 10.0f);
        ImGui::SliderFloat3("Direction", &m_globalConstBufferCPU.light.direction.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Coefficient", &m_globalConstBufferCPU.light.coefficient, 0.0f, 10.0f);
    }

    if(ImGui::CollapsingHeader("Model"))
    {
        ImGui::SliderFloat("Roughness", &spaceship->m_modelBufferCPU.material.roughness, 0.0f, 0.5f);
        ImGui::SliderFloat("Metaillic", &spaceship->m_modelBufferCPU.material.metallic, 0.0f, 0.5f);
        ImGui::CheckboxFlags("Height Map", &spaceship->m_modelBufferCPU.activeHeightMap, 1);
        ImGui::CheckboxFlags("Normal Map", &spaceship->m_modelBufferCPU.activeNormalMap, 1);
    }
    
    if(ImGui::CollapsingHeader("Sphere"))
    {
        ImGui::SliderFloat("sphere roughness", &sphere->m_modelBufferCPU.material.roughness, 0.0f, 0.5f);
        ImGui::SliderFloat("sphere metaillic", &sphere->m_modelBufferCPU.material.metallic, 0.0f, 0.5f);
        ImGui::CheckboxFlags("sphere turn on height map", &sphere->m_modelBufferCPU.activeHeightMap, 1);
        ImGui::CheckboxFlags("sphere turn on normal map", &sphere->m_modelBufferCPU.activeNormalMap, 1);
    }
    
    ImGui::End();
}


float Graphics::GetAspectRatio()
{
    return float(m_width) / m_height;
}

void Graphics::ProcessMouseMove(const int xPos, const int yPos)
{   // From : [0, 0] ~ [width-1, height-1]
    // To : [-1, -1] ~ [1, 1]
    const float ndcX = (xPos * 2.0f / m_width) - 1.0f;
    const float ndcY = -((yPos * 2.0f / m_height) - 1.0f);
    // traversal mode
    if (m_fpvMode)
    {    
        cam->RotateFromMouse(ndcX, ndcY);

        SetCursorPos(m_aimPoint.x, m_aimPoint.y);
    }
    // edit mode
    else
    {
        if (m_leftClick)
        {
            if (m_picking && m_pickingObject != nullptr) // on same plane
            {
                Matrix view = cam->GetViewMatrix();
                Matrix projection = cam->GetProjectionMatrix();
                Matrix unprojection = projection.Invert() * view.Invert();

                Vector3 p0 = Vector3(ndcX, ndcY, 0.0f); 
                Vector3 p1 = Vector3(ndcX, ndcY, 1.0f);

                p0 = Vector3::Transform(p0, unprojection);
                p1 = Vector3::Transform(p1, unprojection);
                Vector3 direction = p1 - p0;
                direction.Normalize();

                Ray ray = Ray(p0, direction);
                float distance = 0.0f;
                ray.Intersects(m_draggingPlane, distance);

                Vector3 hitPoint = p0 + direction * distance;
                Vector3 dv = hitPoint - prevHit;

                m_pickingObject->UpdateWorldMatrix(m_pickingObject->GetWorldMatrix() * Matrix::CreateTranslation(dv));
                m_pickingObject->UpdateBuffer(m_context);
                m_pickingObject->m_boundingSphere.Center = m_pickingObject->m_boundingSphere.Center + dv;

                pickingEffect->UpdateWorldMatrix(Matrix::CreateTranslation(hitPoint));
                pickingEffect->UpdateBuffer(m_context);

                prevHit = hitPoint;
            }
            else
            {
                Vector3 p0 = Vector3(ndcX, ndcY, 0.0f);
                Vector3 p1 = Vector3(ndcX, ndcY, 1.0f);
                // NDC -> View -> World
                Matrix unprojection = cam->GetProjectionMatrix().Invert() * cam->GetViewMatrix().Invert();

                p0 = Vector3::Transform(p0, unprojection);
                p1 = Vector3::Transform(p1, unprojection);

                Vector3 direction = p1 - p0;
                direction.Normalize();

                Ray ray = Ray(p0, direction);

                for (Object* object : objects)
                {
                    if (!object->hitEnabled) continue;
                    float distance = 0.0f;
                    bool hit = ray.Intersects(object->m_boundingSphere, distance);

                    if (hit)
                    {
                        if (!m_picking)
                        {
                            m_picking = true;
                            m_pickingObject = object;
                        }
                        Vector3 hitPoint = p0 + direction * distance;
                        Vector3 n = -cam->GetDirection();
                        n.Normalize();

                        prevHit = hitPoint;
                        m_draggingPlane = Plane(hitPoint, n);

                        pickingEffect->UpdateWorldMatrix(Matrix::CreateTranslation(hitPoint));
                        pickingEffect->UpdateBuffer(m_context);
                        break;
                    }
                }
            }
        }
        else
        {
            m_picking = false;
            m_pickingObject = nullptr;
        }
    }
}

void Graphics::ProcessMouseWheel(const int wheel)
{
    const int speed = 2;
    if (m_leftClick && m_picking)
    {
        int movement = wheel / 120; 
        Vector3 dv = spaceship
            ->m_boundingSphere.Center - cam->GetOrigin();
        dv.Normalize();
        dv = dv * movement * speed;

        Matrix m = Matrix::CreateTranslation(dv);
        spaceship->UpdateWorldMatrix(spaceship->GetWorldMatrix() * m);
        spaceship->UpdateBuffer(m_context);
        spaceship->m_boundingSphere.Center = spaceship->m_boundingSphere.Center + dv;

        pickingEffect->UpdateWorldMatrix(pickingEffect->GetWorldMatrix() * m);
        pickingEffect->UpdateBuffer(m_context);

        Vector3 n = -cam->GetDirection();
        n.Normalize();
        m_draggingPlane = Plane(spaceship->m_boundingSphere.Center, n);

        prevHit = prevHit + dv;
    }
}
void Graphics::UpdateCameraPosition(float dt)
{
    if (!m_fpvMode) return;

    cam->SetRunVars(m_keyState[VK_SHIFT]);
    if (m_keyState[0x57]) // W
    {
        cam->MoveForward(dt);
    }
    if (m_keyState[0x41]) // A
    {
        cam->MoveRight(-dt);
    }
    if (m_keyState[0x53]) // S
    {
        cam->MoveForward(-dt);
    }
    if (m_keyState[0x44]) // D
    {
        cam->MoveRight(dt);
    }
}

void Graphics::ToggleFPVMode()
{
    if (!m_fpvMode)
    {
        m_fpvMode = true;
        SetCursorPos(m_aimPoint.x, m_aimPoint.y);
        ShowCursor(false);
    }
    else
    {
        m_fpvMode = false;
        ShowCursor(true);
    }
}

void Graphics::ToneMappingSetUp()
{

    // Rasterizer State
    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    rastDesc.FrontCounterClockwise = false;
    rastDesc.DepthClipEnable = false; 

    ThrowIfFailed(m_device->CreateRasterizerState(&rastDesc, m_toneState.GetAddressOf()));
}


Graphics::~Graphics()
{
    delete cam;
    delete sphere;
    delete spaceship;
    delete envMap;
    delete m_copySquare;
}


