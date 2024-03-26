#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h> 
#include <directxtk/SimpleMath.h>

#include "Util.h"
#include "Mesh.h"

using Microsoft::WRL::ComPtr;

class EnvMap
{
public:
    EnvMap(const std::string name, const std::wstring filepath);

    void Init(ComPtr<ID3D11Device>& device);
    void Render(ComPtr<ID3D11DeviceContext>& context);
    void SetIBLSRVs(ComPtr<ID3D11DeviceContext>& context);
    ~EnvMap();

private:
    Mesh* m_mesh;
    std::string m_name;
    std::wstring m_filepath;

    ComPtr<ID3D11ShaderResourceView> m_envSRV;
    // For IBL
    ComPtr<ID3D11ShaderResourceView> m_irradianceSRV;
    ComPtr<ID3D11ShaderResourceView> m_specularSRV;
    ComPtr<ID3D11ShaderResourceView> m_brdfLookUpSRV;

};