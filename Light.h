#pragma once

#include <directxtk/SimpleMath.h>

struct Light
{
	DirectX::SimpleMath::Vector3 pos;
	float strength;
	DirectX::SimpleMath::Vector3 direction;
	float coefficient;
};