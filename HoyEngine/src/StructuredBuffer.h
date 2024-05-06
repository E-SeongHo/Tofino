#pragma once

#include <cassert>
#include <vector>

#include "Buffer.h"

namespace Tofino
{
	template<typename T>
	class StructuredBuffer : public Buffer
	{
	public:
		StructuredBuffer(UINT bindFlag, UINT slot, UINT numElements)
			: m_bindFlag(bindFlag), m_slot(slot)
		{
            m_data.resize(numElements);
		}

		void Init(ComPtr<ID3D11Device>& device) override
		{
            D3D11_BUFFER_DESC bufferDesc;
            ZeroMemory(&bufferDesc, sizeof(bufferDesc));
            bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            bufferDesc.ByteWidth = m_data.size() * sizeof(T);
            bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;   // Vertex Shader 
            bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            bufferDesc.StructureByteStride = sizeof(T);
            bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

            D3D11_SUBRESOURCE_DATA initData;
            ZeroMemory(&initData, sizeof(initData));
            initData.pSysMem = m_data.data();
            initData.SysMemPitch = 0;
            initData.SysMemSlicePitch = 0;
            ThrowIfFailed(device->CreateBuffer(&bufferDesc, &initData, m_buffer.GetAddressOf()));


            // To expand using this structured buffer in Compute Shader
            /*bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            NO CPUAccessFlags
            bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;*/

           /* D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
            ZeroMemory(&uavDesc, sizeof(uavDesc));
            uavDesc.Format = DXGI_FORMAT_UNKNOWN;
            uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
            uavDesc.Buffer.NumElements = m_data.size();
            device->CreateUnorderedAccessView(m_buffer.Get(), &uavDesc, m_uav.GetAddressOf());*/

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            ZeroMemory(&srvDesc, sizeof(srvDesc));
            srvDesc.Format = DXGI_FORMAT_UNKNOWN;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
            srvDesc.BufferEx.NumElements = m_data.size();
            device->CreateShaderResourceView(m_buffer.Get(), &srvDesc, m_srv.GetAddressOf());
		}

        void Update(ComPtr<ID3D11DeviceContext>& context)
        {
            D3D11_MAPPED_SUBRESOURCE resource;
            context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
            memcpy(resource.pData, m_data.data(), m_data.size() * sizeof(T));
            context->Unmap(m_buffer.Get(), 0);
        }

		void Bind(ComPtr<ID3D11DeviceContext>& context) const override
		{
            assert(m_bindFlag != NONE);

            if ((m_bindFlag & VERTEX_SHADER) == VERTEX_SHADER)
            {
                context->VSSetShaderResources(m_slot, 1, m_srv.GetAddressOf());
            }

            if ((m_bindFlag & GEOMETRY_SHADER) == GEOMETRY_SHADER)
            {
                context->GSSetShaderResources(m_slot, 1, m_srv.GetAddressOf());
            }

            if ((m_bindFlag & PIXEL_SHADER) == PIXEL_SHADER)
            {
                context->PSSetShaderResources(m_slot, 1, m_srv.GetAddressOf());
            }

            if((m_bindFlag & COMPUTE_SHADER) == COMPUTE_SHADER)
            {
                context->CSSetUnorderedAccessViews(m_slot, 1, m_uav.GetAddressOf(), NULL);
            }
		}

        std::vector<T>& GetData() { return m_data; }

	private:
		std::vector<T> m_data;
		UINT m_bindFlag = NONE;
		UINT m_slot;

        ComPtr<ID3D11ShaderResourceView> m_srv;
        ComPtr<ID3D11UnorderedAccessView> m_uav;
	};

	
}



