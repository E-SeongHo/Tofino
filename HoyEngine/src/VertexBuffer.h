#pragma once

#include <vector>

#include "SimpleMath.h"
#include "Buffer.h"

namespace Tofino
{
	struct Vertex
	{
		Vector3 pos;
		Vector3 color;
		Vector3 normal;
		Vector2 uv;
		Vector3 tangent;
	};

	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer() = default;
		VertexBuffer(std::vector<Vertex>& vertices);
		~VertexBuffer() override {}

		void Init(ComPtr<ID3D11Device>& device) override;

		void Bind(ComPtr<ID3D11DeviceContext>& context) const override;

		const std::vector<Vertex>& GetData() const { return m_vertices; }

	private:
		std::vector<Vertex> m_vertices;
	};
}
