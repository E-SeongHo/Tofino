#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>
#include <directxtk/DDSTextureLoader.h>
#include <string>

#include "Debug.h"

using Microsoft::WRL::ComPtr;

class TextureLoader
{
public:
    static void CreateCubemapTexture(ComPtr<ID3D11Device>& device, const std::wstring filename, ComPtr<ID3D11ShaderResourceView>& srv)
    {
        using namespace DirectX;
        ComPtr<ID3D11Texture2D> texture;

        // https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
        ThrowIfFailed(CreateDDSTextureFromFileEx(
            device.Get(), filename.c_str(), 0, D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE, 0,
            D3D11_RESOURCE_MISC_TEXTURECUBE,
            DDS_LOADER_FLAGS(false), (ID3D11Resource**)texture.GetAddressOf(),
            srv.GetAddressOf(), nullptr));
    }
};