#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>
#include <directxtk/DDSTextureLoader.h>
#include <string>

#include "Util.h"

using Microsoft::WRL::ComPtr;

class TextureLoader
{
public:
    static void CreateDDSCubemapTexture(ComPtr<ID3D11Device>& device, const std::wstring filename, 
        ComPtr<ID3D11ShaderResourceView>& shaderResourceView);
    static void CreateDDSTexture(ComPtr<ID3D11Device>& device, const std::wstring filename,
        ComPtr<ID3D11ShaderResourceView>& shaderResourceView);
};
