#pragma once

#include "SimpleMath.h"

namespace Tofino
{
	struct Transform
	{
	public:
		Vector3 Location = Vector3(0.0f);
		Vector3 Rotation = Vector3(0.0f);
		Vector3 Scale = Vector3(1.0f);
	};
}