#pragma once

#include <directxtk/SimpleMath.h>

enum class LightType
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

struct Light // 16byte allign
{
	// LightType type;
	DirectX::SimpleMath::Vector3 pos;
	float strength;
	DirectX::SimpleMath::Vector3 direction;
	float coefficient;
};