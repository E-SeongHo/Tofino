#pragma once

#include <functional>

#include "AABB.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"

namespace Tofino
{
	class Mesh;

	struct ColliderBuffer
	{
		Matrix world;
		Color color;
	};

	class Collider
	{
	public:
		Collider(const Vector3& lowerBound = Vector3(-1.0f), const Vector3& upperBound = Vector3(1.0f));
		~Collider() = default;

		void Bind(ComPtr<ID3D11DeviceContext>& context) const
		{
			m_vertexBuffer.Bind(context);
			m_constBuffer.Bind(context);
		}

		void Translate(const Vector3& translation);
		void Scale(const Vector3& scale);

		// this called by scene system
		void UpdateConstBuffer(ComPtr<ID3D11DeviceContext>& context);

		bool CheckCollision(const Collider& other) const;

		void BindCollisionEvent(const std::function<void()>& eventFunction) { m_collisionFunction = eventFunction; }
		void OnCollisionDetected();

		bool m_isColliding = false;
	private:
		AABB m_boundingBox;
		std::function<void()> m_collisionFunction;

		VertexBuffer m_vertexBuffer;
		ConstantBuffer<ColliderBuffer> m_constBuffer;
	};
}

