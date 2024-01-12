#pragma once

#include <directxtk/SimpleMath.h>

struct Material
{
	DirectX::SimpleMath::Vector3 albedo = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	float roughness = 0.1f;
	float metallic = 0.1f;
	float padding[3];
}; 