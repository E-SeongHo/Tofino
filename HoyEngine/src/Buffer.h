#pragma once

#include <iostream>

#include "Helper.h"

class Buffer
{
public:
	Buffer() = default;
	virtual ~Buffer() {}

	virtual void Init(ComPtr<ID3D11Device>& device) = 0;
	virtual void Bind(ComPtr<ID3D11DeviceContext>& context) = 0;

	ComPtr<ID3D11Buffer> GetBuffer() { return m_buffer; }

protected:
	ComPtr<ID3D11Buffer> m_buffer;
};

