#pragma once

#include <directxtk/SimpleMath.h>

#include "Util.h"

using Microsoft::WRL::ComPtr;

template<typename T>
class ConstantBuffer
{
public:
	void Init(ComPtr<ID3D11Device>& device)
	{
		Util::CreateConstantBuffer(device, *m_data, m_buffer);
	}
	
	void Update(ComPtr<ID3D11DeviceContext>& context)
	{
		Util::UpdateConstantBuffer(context, *m_data, m_buffer);
	}

	T GetData() { return *m_data; }

	ComPtr<ID3D11Buffer> GetBuffer() { return m_buffer; }

private:
	T* m_data;
	ComPtr<ID3D11Buffer> m_buffer;
};

