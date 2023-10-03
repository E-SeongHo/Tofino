#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>
#include <windows.h> 
#include <exception>
#include <iostream>

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


}
