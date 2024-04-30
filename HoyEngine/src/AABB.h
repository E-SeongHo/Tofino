#pragma once

#include "SimpleMath.h"

namespace Tofino
{
	class AABB
	{
	public:
		AABB() = default;
		AABB(Vector3 lowerBound, Vector3 upperBound)
			: m_lowerBound(lowerBound), m_upperBound(upperBound) {}

		float GetArea() const
		{
			Vector3 dist = m_upperBound - m_lowerBound;
			return 2.0f * (dist.x * dist.y + dist.y * dist.z + dist.z * dist.x);
		}
		AABB Combine(const AABB& other);

	private:
		Vector3 m_lowerBound;
		Vector3 m_upperBound;
	};
	
}