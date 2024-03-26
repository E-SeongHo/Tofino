#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>
#include <windows.h> 
#include <exception>
#include <iostream>
#include <vector>

inline void ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr)) {
        // Set a breakpoint on this line to catch Win32 API errors.
        throw std::exception();
    }
}

namespace Util
{
    using Microsoft::WRL::ComPtr;
    template<typename Buffer>
    void CreateConstantBuffer(ComPtr<ID3D11Device>& device, const Buffer& cbufferData, ComPtr<ID3D11Buffer>& constantBuffer)
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

    template<typename Buffer>
    void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context, const Buffer& updateData, ComPtr<ID3D11Buffer>& constantBuffer)
    {
        D3D11_MAPPED_SUBRESOURCE resource;
        context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
        memcpy(resource.pData, &updateData, sizeof(updateData));
        context->Unmap(constantBuffer.Get(), 0);
    }

    template<typename T_Vertex>
    void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const std::vector<T_Vertex>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer)
    {
        // D3D11_USAGE enumeration (d3d11.h)
        // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_usage
        D3D11_BUFFER_DESC ds;
        ZeroMemory(&ds, sizeof(ds));
        ds.Usage = D3D11_USAGE_IMMUTABLE;
        ds.ByteWidth = UINT((sizeof(T_Vertex)) * vertices.size());
        ds.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        ds.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
        ds.StructureByteStride = sizeof(T_Vertex);

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = vertices.data();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;

        ThrowIfFailed(device->CreateBuffer(&ds, &data, vertexBuffer.GetAddressOf()));
    }

    void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const std::vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer);
}