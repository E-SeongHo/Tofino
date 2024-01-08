#pragma once

#include <directxtk/SimpleMath.h>

struct Material
{
	DirectX::SimpleMath::Vector3 albedo;
	float roughness;
	float metallic;
	float padding[3];
};