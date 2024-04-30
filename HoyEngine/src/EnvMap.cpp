
#include <string>
#include <DirectXCollision.h>

#include "EnvMap.h"
#include "Mesh.h"
#include "MeshLoader.h"

namespace Tofino
{
    EnvMap::EnvMap(const std::string name, const std::string filepath)
        : m_name(name), m_filepath(filepath)
    {
        // trivial copy overhead 
        Mesh meshData = MeshLoader::LoadCube(500.0f)[0];

        std::vector<Vertex> vertices(meshData.GetVertexBuffer().GetData());
        std::vector<uint32_t> indices(meshData.GetIndexBuffer().GetData());

        std::reverse(indices.begin(), indices.end());

        m_vertexBuffer = VertexBuffer(vertices);
        m_indexBuffer = IndexBuffer(indices);
    }

    void EnvMap::Init(ComPtr<ID3D11Device>& device)
    {
        m_vertexBuffer.Init(device);
        m_indexBuffer.Init(device);

        size_t idx = m_filepath.rfind("/", m_filepath.size() - 2);
        std::string prefix = m_filepath.substr(idx);
        prefix = prefix.substr(1, prefix.size() - 2);

        const std::string envFilename = m_filepath + prefix + "EnvHDR.dds";
        const std::string irradianceFilename = m_filepath + prefix + "DiffuseHDR.dds";
        const std::string specularFilename = m_filepath + prefix + "SpecularHDR.dds";
        const std::string brdfFilename = m_filepath + prefix + "Brdf.dds";

        m_envMap.Load(device, envFilename, TextureType::CUBE);
        m_irradianceMap.Load(device, irradianceFilename, TextureType::CUBE);
        m_specularMap.Load(device, specularFilename, TextureType::CUBE);
        m_brdfLUT.Load(device, brdfFilename, TextureType::DEFAULT);
    }

    void EnvMap::Bind(ComPtr<ID3D11DeviceContext>& context) const
    {
        context->PSSetShaderResources(0, 1, m_envMap.GetSRV().GetAddressOf());

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetBuffer().GetAddressOf(), &stride, &offset);
        context->IASetIndexBuffer(m_indexBuffer.GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
    }

    void EnvMap::BindIBLSRVs(ComPtr<ID3D11DeviceContext>& context) const
    {
        ID3D11ShaderResourceView* IBLTextures[] =
        { m_irradianceMap.GetSRV().Get(), m_specularMap.GetSRV().Get(), m_brdfLUT.GetSRV().Get() };

        context->PSSetShaderResources(10, 3, IBLTextures);
    }

}
