#pragma once

#include <vector>

#include "Buffer.h"

namespace Tofino
{
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(std::vector<uint32_t> indices);
		~IndexBuffer() override {}

		void Init(ComPtr<ID3D11Device>& device) override;

		void Bind(ComPtr<ID3D11DeviceContext>& context) const override;

		const std::vector<uint32_t>& GetData() const { return m_indices; }

	private:
		std::vector<uint32_t> m_indices;
	};

}