#pragma once

#include <vector>

#include "Buffer.h"

struct Vertex
{
	DirectX::SimpleMath::Vector3 pos;
	DirectX::SimpleMath::Vector3 color;
	DirectX::SimpleMath::Vector3 normal;
	DirectX::SimpleMath::Vector2 uv;
	DirectX::SimpleMath::Vector3 tangent;
};

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(std::vector<Vertex>& vertices);
	~VertexBuffer() override {}

	void Init(ComPtr<ID3D11Device>& device) override;

	void Bind(ComPtr<ID3D11DeviceContext>& context) override;

	std::vector<Vertex>& GetData();

private:
	std::vector<Vertex> m_vertices;
};

