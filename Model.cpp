
#include <vector>

#include "Model.h"

using namespace std;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;


void Geometry::CreateBuffers(ComPtr<ID3D11Device>& device)
{
    Geometry::CreateVertexBuffer(device, m_vertices, m_vertexBuffer);
    Geometry::CreateIndexBuffer(device, m_indices, m_indexBuffer);
    Geometry::CreateConstantBuffer(device, m_constBufferCPU, m_constBufferGPU);
}

void Geometry::Render(ComPtr<ID3D11DeviceContext>& context)
{
    context->VSSetConstantBuffers(0, 1, m_constBufferGPU.GetAddressOf()); // TODO : global const »ý°¢

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
    context->DrawIndexed(m_indexCount, 0, 0);
}  


void Triangle::Init() 
{
    m_vertices.push_back(Vertex{ Vector3{ -0.8f, -0.8f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });
    m_vertices.push_back(Vertex{ Vector3{ 0.0f, 0.8f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });
    m_vertices.push_back(Vertex{ Vector3{ 0.8f, -0.8f, 1.0f }, Vector3{ 1.0f, 0.0f, 0.0f } });

    m_indices = { 0, 1, 2 };
    m_indexCount = m_indices.size();

    m_constBufferCPU.world = Matrix();
    m_constBufferCPU.view = Matrix();
    m_constBufferCPU.projection = Matrix();

}

