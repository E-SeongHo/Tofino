#pragma once

#include "SimpleMath.h"

namespace Tofino
{
	enum class LightType
	{
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
	};

	struct Light // 16byte allign
	{
		// LightType type;
		Vector3 pos;
		float strength;
		Vector3 direction;
		float coefficient;
	};
}