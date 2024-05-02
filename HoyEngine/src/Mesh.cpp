#include "Mesh.h"

namespace Tofino
{
    using DirectX::BoundingSphere;

    Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
        : m_vertexBuffer(vertices), m_indexBuffer(indices),
        m_material(VERTEX_SHADER | PIXEL_SHADER, 1)
    {
    }

    void Mesh::Init(ComPtr<ID3D11Device>& device)
    {
        // Init buffers
        m_vertexBuffer.Init(device);
        m_indexBuffer.Init(device);
        m_material.Init(device);
    }

    void Mesh::UpdateBuffer(ComPtr<ID3D11DeviceContext>& context)
    {
        m_material.Update(context);
        m_updateFlag = false;
    }

    void Mesh::Bind(ComPtr<ID3D11DeviceContext>& context) const
    {
        m_material.Bind(context);
        m_vertexBuffer.Bind(context);
        m_indexBuffer.Bind(context);
    }

    void Mesh::SetMaterialFactors(Vector4 baseColor, float roughness, const float metallic)
    {
        m_material.SetFactors(baseColor, roughness, metallic);
        m_updateFlag = true;
    }
}