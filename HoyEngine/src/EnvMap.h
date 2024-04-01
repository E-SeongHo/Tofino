#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>

#include "Geometry.h"

using Microsoft::WRL::ComPtr;

class EnvMap
{
public:
    EnvMap(const std::string name, const std::string filepath);

    void Init(ComPtr<ID3D11Device>& device);
    void Render(ComPtr<ID3D11DeviceContext>& context);
    void BindIBLSRVs(ComPtr<ID3D11DeviceContext>& context);
    ~EnvMap();

private:
    Cube m_shape;
    std::string m_name;
    std::string m_filepath;

    Texture m_envMap;
    // IBL
    Texture m_irradianceMap;
    Texture m_specularMap;
    Texture m_brdfLUT;
};