#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>
#include <vector>
#include <string>

#include "Util.h"


struct Vertex
{
    DirectX::SimpleMath::Vector3 pos;
    DirectX::SimpleMath::Vector3 color;
    DirectX::SimpleMath::Vector3 normal;
    DirectX::SimpleMath::Vector2 uv;
};

// 16Byte align
struct ModelBuffer // Must Store as a Column Matrix 
{
    DirectX::SimpleMath::Matrix world; // equal model matrix 
    DirectX::SimpleMath::Matrix worldIT; 
};

using Microsoft::WRL::ComPtr;
class Geometry
{
protected: 
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    ComPtr<ID3D11Buffer> m_constBufferGPU;
    UINT m_indexCount = 0;

    ModelBuffer m_constBufferCPU;

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

public:
    virtual void Init(const float scale = 1.0f) = 0;
    void ReverseIndices();
    
    void CreateBuffers(ComPtr<ID3D11Device>& device);
    void UpdateBuffer(ComPtr<ID3D11DeviceContext>& context);
    void Render(ComPtr<ID3D11DeviceContext>& context);
    void RenderNormal(ComPtr<ID3D11DeviceContext>& context);
    void CopySquareRenderSetup(ComPtr<ID3D11DeviceContext>& context);
    DirectX::SimpleMath::Matrix GetWorldMatrix();

    void UpdateWorldMatrix(DirectX::SimpleMath::Matrix worldColumn);

protected:
    static void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const std::vector<Vertex>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer)
    {
        // D3D11_USAGE enumeration (d3d11.h)
        // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_usage
        D3D11_BUFFER_DESC ds;
        ZeroMemory(&ds, sizeof(ds));
        ds.Usage = D3D11_USAGE_IMMUTABLE;
        ds.ByteWidth = UINT((sizeof(Vertex)) * vertices.size());
        ds.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        ds.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
        ds.StructureByteStride = sizeof(Vertex);

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices.data();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;

        ThrowIfFailed(device->CreateBuffer(&ds, &data, vertexBuffer.GetAddressOf()));
    }
    static void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const std::vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
    {
        D3D11_BUFFER_DESC ds = {};
        // ZeroMemory(&ds, sizeof(ds));
        ds.Usage = D3D11_USAGE_IMMUTABLE;
        ds.ByteWidth = UINT(indices.size() * sizeof(uint32_t));
        ds.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ds.CPUAccessFlags = 0;
        ds.StructureByteStride = sizeof(uint32_t);

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = indices.data();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;

        ThrowIfFailed(device->CreateBuffer(&ds, &data, indexBuffer.GetAddressOf()));
    }
};

class Triangle : public Geometry
{
public:
    void Init(const float scale = 1.0f) override;
};

class Square : public Geometry
{
public:
    void Init(const float scale = 1.0f) override;
};

class Cube : public Geometry
{
public:
    void Init(const float scale = 1.0f) override;
};


class EnvMap 
{
private:
    Geometry* m_mesh = nullptr;

    ComPtr<ID3D11ShaderResourceView> m_envSRV;
    // For IBL
    ComPtr<ID3D11ShaderResourceView> m_irradianceSRV;
    ComPtr<ID3D11ShaderResourceView> m_specularSRV;
    ComPtr<ID3D11ShaderResourceView> m_brdfLookUpSRV;

public:
    void Init(ComPtr<ID3D11Device>& device, const std::wstring filepath);
    void Render(ComPtr<ID3D11DeviceContext>& context);
    void func(ComPtr<ID3D11DeviceContext>& context);
    ~EnvMap();
};