#include "IndexBuffer.h"

namespace Tofino
{
    IndexBuffer::IndexBuffer(std::vector<uint32_t> indices)
        : m_indices(std::move(indices))
    {
    }

    void IndexBuffer::Init(ComPtr<ID3D11Device>& device)
    {
        D3D11_BUFFER_DESC ds = {};
        // ZeroMemory(&ds, sizeof(ds));
        ds.Usage = D3D11_USAGE_IMMUTABLE;
        ds.ByteWidth = UINT(m_indices.size() * sizeof(uint32_t));
        ds.BindFlags = D3D11_BIND_INDEX_BUFFER;
        ds.CPUAccessFlags = 0;
        ds.StructureByteStride = sizeof(uint32_t);

        D3D11_SUBRESOURCE_DATA data = { 0 };
        data.pSysMem = m_indices.data();
        data.SysMemPitch = 0;
        data.SysMemSlicePitch = 0;

        ThrowIfFailed(device->CreateBuffer(&ds, &data, m_buffer.GetAddressOf()));
    }

    void IndexBuffer::Bind(ComPtr<ID3D11DeviceContext>& context) const
    {
        context->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    }
}