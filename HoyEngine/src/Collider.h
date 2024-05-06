#pragma once

#include <functional>

#include "AABB.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"

namespace Tofino
{
	struct PhysicsComponent;
	class Mesh;

	struct ColliderBuffer
	{
		Matrix world;
		Color color;
	};
	static_assert((sizeof(ColliderBuffer) % 16) == 0);

	struct Collision
	{
		Vector3 velocity;
		float mass;
		float dt;
	};

	struct CollisionPair
	{
		PhysicsComponent* actor;
		Collision collision;
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

		bool CheckCollision(Collider& other);

		void BindCollisionEvent(const std::function<void(Collision&)>& callback) { m_collisionCallback = callback; }
		void OnCollisionDetected(Collision& collision);

		Vector3 GetLowerBound() { return Vector3::Transform(m_boundingBox.m_lowerBound, m_constBuffer.GetData().world.Transpose()); }
		Vector3 GetUpperBound() { return Vector3::Transform(m_boundingBox.m_upperBound, m_constBuffer.GetData().world.Transpose()); }

	private:
		AABB m_boundingBox;
		bool m_isColliding = false;
		std::function<void(Collision&)> m_collisionCallback;

		VertexBuffer m_vertexBuffer;
		ConstantBuffer<ColliderBuffer> m_constBuffer;
	};
}

