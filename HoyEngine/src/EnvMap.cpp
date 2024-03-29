
#include "EnvMap.h"
#include "Geometry.h"
#include "TextureLoader.h"

#include <string>
#include <DirectXCollision.h>

using namespace std;

EnvMap::EnvMap(const std::string name, const std::wstring filepath)
{
    m_name = name;
    m_filepath = filepath;
}

void EnvMap::Init(ComPtr<ID3D11Device>& device)
{
    m_shape.LoadGeometry(200.0f);

    Mesh& mesh = m_shape.m_meshes[0];
    std::reverse(mesh.m_indexBuffer.GetData().begin(), mesh.m_indexBuffer.GetData().end());

    size_t idx = m_filepath.rfind(L"/", m_filepath.size() - 2);
    wstring prefix = m_filepath.substr(idx);
    prefix = prefix.substr(1, prefix.size() - 2);

    const wstring envFilename = m_filepath + prefix + L"EnvHDR.dds";
    const wstring irradianceFilename = m_filepath + prefix + L"DiffuseHDR.dds";
    const wstring specularFilename = m_filepath + prefix + L"SpecularHDR.dds";
    const wstring brdfFilename = m_filepath + prefix + L"Brdf.dds";

    TextureLoader::CreateDDSCubemapTexture(device, envFilename, m_envSRV);
    TextureLoader::CreateDDSCubemapTexture(device, irradianceFilename, m_irradianceSRV);
    TextureLoader::CreateDDSCubemapTexture(device, specularFilename, m_specularSRV);
    TextureLoader::CreateDDSTexture(device, brdfFilename, m_brdfLookUpSRV);

    m_shape.Init(device);
}

void EnvMap::Render(ComPtr<ID3D11DeviceContext>& context)
{
    m_shape.GetConstBuffer().Bind(context);

    context->PSSetShaderResources(0, 1, m_envSRV.GetAddressOf());

    m_shape.m_meshes[0].Render(context);
}

void EnvMap::SetIBLSRVs(ComPtr<ID3D11DeviceContext>& context)
{
    ID3D11ShaderResourceView* IBLTextures[] =
    { m_irradianceSRV.Get(), m_specularSRV.Get(), m_brdfLookUpSRV.Get() };

    context->PSSetShaderResources(10, 3, IBLTextures);
}

EnvMap::~EnvMap()
{
}