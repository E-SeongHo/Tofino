#pragma once

#include "Material.h"
#include "Util.h"

struct Vertex
{
    DirectX::SimpleMath::Vector3 pos;
    DirectX::SimpleMath::Vector3 color;
    DirectX::SimpleMath::Vector3 normal;
    DirectX::SimpleMath::Vector2 uv;
    DirectX::SimpleMath::Vector3 tangent;
};

// 16Byte align
struct ModelBuffer // Must Store as a Column Matrix 
{
    DirectX::SimpleMath::Matrix world; // equal model matrix 
    DirectX::SimpleMath::Matrix worldIT;
    Material material; // 32bytes
};

class Hittable
{
public:
    bool onActive = false;
    DirectX::BoundingSphere m_boundingSphere;
};

using Microsoft::WRL::ComPtr;
class Mesh : public Hittable
{
public:
    void CreateBuffers(ComPtr<ID3D11Device>& device);
    void UpdateBuffer(ComPtr<ID3D11DeviceContext>& context);
    void Render(ComPtr<ID3D11DeviceContext>& context);
    void RenderNormal(ComPtr<ID3D11DeviceContext>& context);
    void CopySquareRenderSetup(ComPtr<ID3D11DeviceContext>& context);
    DirectX::SimpleMath::Matrix GetWorldMatrix();

    void UpdateWorldMatrix(DirectX::SimpleMath::Matrix worldRow);

    void LoadDDSTexture(ComPtr<ID3D11Device>& device, const std::wstring filepath);
    void LoadTexture(ComPtr<ID3D11Device>& device, const std::string filepath, ComPtr<ID3D11ShaderResourceView>& srv, const bool gammaDecode);
    void SetSRVs(ComPtr<ID3D11DeviceContext>& context);

public:
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    ComPtr<ID3D11Buffer> m_constBufferGPU;
    UINT m_indexCount = 0;

    ModelBuffer m_constBufferCPU;

    // Meshes
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    // Textures
    std::string m_diffuseFilename;
    std::string m_normalFilename;
    ComPtr<ID3D11ShaderResourceView> m_diffuseSRV;
    ComPtr<ID3D11ShaderResourceView> m_normalSRV;
};