#include "Mesh.h"

using namespace std;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Matrix;
using DirectX::BoundingSphere;

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
    : m_vertexBuffer(vertices), m_indexBuffer(indices),
    m_material(VERTEX_SHADER | PIXEL_SHADER, 1)
{
    m_indexCount = (UINT)indices.size();
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

void Mesh::Render(ComPtr<ID3D11DeviceContext>& context)
{
    m_material.Bind(context);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetBuffer().GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
    context->DrawIndexed(m_indexCount, 0, 0);
}

void Mesh::RenderNormal(ComPtr<ID3D11DeviceContext>& context)
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetBuffer().GetAddressOf(), &stride, &offset);
    //context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    context->Draw(UINT(m_vertexBuffer.GetData().size()), 0);
}

void Mesh::CopySquareRenderSetup(ComPtr<ID3D11DeviceContext>& context)
{
    // PS SET CONSTANT ( ex) gamma )
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetBuffer().GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

    context->DrawIndexed(m_indexCount, 0, 0);
}

bool Mesh::IsUpdateFlagSet()
{
    return m_updateFlag;
}

Material& Mesh::GetMaterial()
{
    return m_material;
}

void Mesh::SetMaterialFactors(DirectX::SimpleMath::Vector4 baseColor, const float roughness, const float metallic)
{
    m_material.SetFactors(baseColor, roughness, metallic);
    m_updateFlag = true;
}