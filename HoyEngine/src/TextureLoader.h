#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>
#include <directxtk/DDSTextureLoader.h>
#include <directxtk/WICTextureLoader.h>
#include <string>
#include <vector>

#include "Util.h"


namespace TextureLoader
{
    using Microsoft::WRL::ComPtr;

    void CreateDDSCubemapTexture(ComPtr<ID3D11Device>& device, const std::wstring filename, 
        ComPtr<ID3D11ShaderResourceView>& shaderResourceView);
    void CreateDDSTexture(ComPtr<ID3D11Device>& device, const std::wstring filename,
        ComPtr<ID3D11ShaderResourceView>& shaderResourceView);
    bool CreateTextureFromImage(ComPtr<ID3D11Device>& device, const std::string filename,
        ComPtr<ID3D11ShaderResourceView>& shaderResourceView, const bool gammaDecode);
};
