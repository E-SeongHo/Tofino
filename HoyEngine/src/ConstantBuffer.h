#pragma once

#include <iostream>
#include <cassert>

#include "Buffer.h"

namespace Tofino
{
	enum CONST_BUFFER_BIND_FLAG
	{
		NONE = 0,
		VERTEX_SHADER = 1 << 0,
		GEOMETRY_SHADER = 1 << 1,
		PIXEL_SHADER = 1 << 2
	};

	template<typename T>
	class ConstantBuffer : public Buffer
	{
	public:
		//ConstantBuffer() = default;
		ConstantBuffer(UINT bindFlag, UINT slot) : m_bindFlag(bindFlag), m_slot(slot) {}

		void Init(ComPtr<ID3D11Device>& device)
		{
			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = sizeof(m_data);
			cbDesc.Usage = D3D11_USAGE_DYNAMIC;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;

			// Fill in the subresource data
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = &m_data;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			ThrowIfFailed(device->CreateBuffer(&cbDesc, &InitData, m_buffer.GetAddressOf()));
		}

		void Update(ComPtr<ID3D11DeviceContext>& context)
		{
			D3D11_MAPPED_SUBRESOURCE resource;
			context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
			memcpy(resource.pData, &m_data, sizeof(m_data));
			context->Unmap(m_buffer.Get(), 0);
		}

		void Bind(ComPtr<ID3D11DeviceContext>& context) const override
		{
			assert(m_bindFlag != NONE);

			if (m_bindFlag == NONE)
			{
				std::cout << "Const buffer bind flag NONE" << std::endl;
				return;
			}

			if ((m_bindFlag & VERTEX_SHADER) == VERTEX_SHADER)
			{
				context->VSSetConstantBuffers(m_slot, 1, m_buffer.GetAddressOf());
			}

			if ((m_bindFlag & GEOMETRY_SHADER) == GEOMETRY_SHADER)
			{
				context->GSSetConstantBuffers(m_slot, 1, m_buffer.GetAddressOf());
			}

			if ((m_bindFlag & PIXEL_SHADER) == PIXEL_SHADER)
			{
				context->PSSetConstantBuffers(m_slot, 1, m_buffer.GetAddressOf());
			}
		}

		T& GetData() { return m_data; }

	private:
		T m_data;
		UINT m_bindFlag = NONE;
		UINT m_slot;
	};

}