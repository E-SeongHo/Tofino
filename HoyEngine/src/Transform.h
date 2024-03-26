#pragma once

#include <directxtk/SimpleMath.h>

struct Transform
{
public:
	DirectX::SimpleMath::Vector3 Location = DirectX::SimpleMath::Vector3(0.0f);
	DirectX::SimpleMath::Vector3 Rotation = DirectX::SimpleMath::Vector3(0.0f);
	DirectX::SimpleMath::Vector3 Scale = DirectX::SimpleMath::Vector3(1.0f);
};	