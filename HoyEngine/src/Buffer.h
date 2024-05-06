#pragma once

#include <iostream>

#include "Helper.h"

namespace Tofino
{
	// ConstBuffer, StructuredBuffer
	enum BUFFER_SHADER_BIND_FLAG
	{
		NONE = 0,
		VERTEX_SHADER = 1 << 0,
		GEOMETRY_SHADER = 1 << 1,
		PIXEL_SHADER = 1 << 2,
		COMPUTE_SHADER = 1 << 3
	};

	class Buffer
	{
	public:
		Buffer() = default;
		virtual ~Buffer() {}

		virtual void Init(ComPtr<ID3D11Device>& device) = 0;
		virtual void Bind(ComPtr<ID3D11DeviceContext>& context) const = 0;

		ComPtr<ID3D11Buffer> GetBuffer() const { return m_buffer; }

	protected:
		ComPtr<ID3D11Buffer> m_buffer;
	};
}