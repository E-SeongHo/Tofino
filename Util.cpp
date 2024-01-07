
#include "Util.h"



void Util::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const std::vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
{
    D3D11_BUFFER_DESC ds = {};
    // ZeroMemory(&ds, sizeof(ds));
    ds.Usage = D3D11_USAGE_IMMUTABLE;
    ds.ByteWidth = UINT(indices.size() * sizeof(uint32_t));
    ds.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ds.CPUAccessFlags = 0;
    ds.StructureByteStride = sizeof(uint32_t);

    D3D11_SUBRESOURCE_DATA data = { 0 };
    data.pSysMem = indices.data();
    data.SysMemPitch = 0;
    data.SysMemSlicePitch = 0;

    ThrowIfFailed(device->CreateBuffer(&ds, &data, indexBuffer.GetAddressOf()));
}

