
#include <directxtk/SimpleMath.h>
#include <vector>

#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(std::vector<Vertex>& vertices)
    : m_vertices(std::move(vertices))
{
}

void VertexBuffer::Init(ComPtr<ID3D11Device>& device) 
{
    // D3D11_USAGE enumeration (d3d11.h)
    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_usage
    D3D11_BUFFER_DESC ds;
    ZeroMemory(&ds, sizeof(ds));
    ds.Usage = D3D11_USAGE_IMMUTABLE;
    ds.ByteWidth = UINT((sizeof(Vertex)) * m_vertices.size());
    ds.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    ds.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
    ds.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA data = { 0 };
    data.pSysMem = m_vertices.data();
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    ThrowIfFailed(device->CreateBuffer(&ds, &data, m_buffer.GetAddressOf()));
}

void VertexBuffer::Bind(ComPtr<ID3D11DeviceContext>& context)
{
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_buffer.GetAddressOf(), &stride, &offset);
}

std::vector<Vertex>& VertexBuffer::GetData()
{
    return m_vertices;
}
