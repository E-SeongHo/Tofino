#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>
#include <vector>

// TODO :: Model에 있는게 안어울림.. -> common 클래스로?
inline void ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr)) {
        // Set a breakpoint on this line to catch Win32 API errors.
        throw std::exception();
    }
}

struct Vertex 
{ // Match with input element
    DirectX::SimpleMath::Vector3 pos;
    DirectX::SimpleMath::Vector3 color;
};

struct MVPBuffer 
{
    DirectX::SimpleMath::Matrix world; // equal model matrix
    DirectX::SimpleMath::Matrix view;
    DirectX::SimpleMath::Matrix projection;
};

using Microsoft::WRL::ComPtr;
class Geometry
{
protected: 
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;
    ComPtr<ID3D11Buffer> m_constBufferGPU;
    UINT m_indexCount;

    MVPBuffer m_constBufferCPU;

    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;

public:
    virtual void Init() = 0;
    void CreateBuffers(ComPtr<ID3D11Device>& device);
    void Render(ComPtr<ID3D11DeviceContext>& context);


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
    static void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const std::vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
    {
        D3D11_BUFFER_DESC ds = {};
        // ZeroMemory(&ds, sizeof(ds));
        ds.Usage = D3D11_USAGE_IMMUTABLE;
        ds.ByteWidth = UINT(indices.size() * sizeof(uint16_t));
        ds.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ds.CPUAccessFlags = 0;
        ds.StructureByteStride = sizeof(uint16_t);

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = indices.data();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;

        ThrowIfFailed(device->CreateBuffer(&ds, &data, indexBuffer.GetAddressOf()));
    }

    static void CreateConstantBuffer(ComPtr<ID3D11Device>& device, const MVPBuffer& cbufferData, ComPtr<ID3D11Buffer>& constantBuffer)
    {
        D3D11_BUFFER_DESC cbDesc;
        cbDesc.ByteWidth = sizeof(cbufferData);
        cbDesc.Usage = D3D11_USAGE_DYNAMIC;
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbDesc.MiscFlags = 0;
        cbDesc.StructureByteStride = 0;

        // Fill in the subresource data.
        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = &cbufferData;
        InitData.SysMemPitch = 0;
        InitData.SysMemSlicePitch = 0;

        ThrowIfFailed(device->CreateBuffer(&cbDesc, &InitData, constantBuffer.GetAddressOf()));
    }
};

class Triangle : public Geometry
{
    void Init() override;
};