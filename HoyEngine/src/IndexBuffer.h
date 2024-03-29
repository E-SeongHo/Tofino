#pragma once

#include <vector>

#include "Buffer.h"

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(std::vector<uint32_t> indices);
	~IndexBuffer() override {}

	void Init(ComPtr<ID3D11Device>& device) override;

	void Bind(ComPtr<ID3D11DeviceContext>& context) override;

	std::vector<uint32_t>& GetData();

private:
	std::vector<uint32_t> m_indices;
};

