
#include "EnvMap.h"
#include "Geometry.h"

#include <string>
#include <DirectXCollision.h>

using namespace std;

EnvMap::EnvMap(const std::string name, const std::string filepath)
{
    m_name = name;
    m_filepath = filepath;
}

void EnvMap::Init(ComPtr<ID3D11Device>& device)
{
    m_shape.LoadGeometry(200.0f);

    Mesh& mesh = m_shape.m_meshes[0];
    std::reverse(mesh.m_indexBuffer.GetData().begin(), mesh.m_indexBuffer.GetData().end());

    size_t idx = m_filepath.rfind("/", m_filepath.size() - 2);
    string prefix = m_filepath.substr(idx);
    prefix = prefix.substr(1, prefix.size() - 2);

    const string envFilename = m_filepath + prefix + "EnvHDR.dds";
    const string irradianceFilename = m_filepath + prefix + "DiffuseHDR.dds";
    const string specularFilename = m_filepath + prefix + "SpecularHDR.dds";
    const string brdfFilename = m_filepath + prefix + "Brdf.dds";

    m_envMap.Load(device, envFilename, TextureType::CUBE);
    m_irradianceMap.Load(device, irradianceFilename, TextureType::CUBE);
    m_specularMap.Load(device, specularFilename, TextureType::CUBE);
    m_brdfLUT.Load(device, brdfFilename, TextureType::DEFAULT);

    m_shape.Init(device);
}

void EnvMap::Render(ComPtr<ID3D11DeviceContext>& context)
{
    m_shape.GetConstBuffer().Bind(context);

    context->PSSetShaderResources(0, 1, m_envMap.GetSRV().GetAddressOf());

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_shape.m_meshes[0].m_vertexBuffer.GetBuffer().GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_shape.m_meshes[0].m_indexBuffer.GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
    context->DrawIndexed(m_shape.m_meshes[0].m_indexCount, 0, 0);
}

void EnvMap::BindIBLSRVs(ComPtr<ID3D11DeviceContext>& context)
{
    ID3D11ShaderResourceView* IBLTextures[] =
    { m_irradianceMap.GetSRV().Get(), m_specularMap.GetSRV().Get(), m_brdfLUT.GetSRV().Get() };

    context->PSSetShaderResources(10, 3, IBLTextures);
}

EnvMap::~EnvMap()
{
}