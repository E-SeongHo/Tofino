#pragma once

#include <directxtk/SimpleMath.h>

struct Material
{
	DirectX::SimpleMath::Vector4 albedo = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	float roughness = 0.1f;
	float metallic = 0.1f;
	float padding[2];
}; 