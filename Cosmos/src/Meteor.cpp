
#include <random>

#include "Meteor.h"
#include "MeshLoader.h"

using namespace Tofino;

Meteor::Meteor(Tofino::Scene* scene, const std::string name, const bool isHittable)
	:Object(scene, name, isHittable)
{
	AddComponent<TransformComponent>();

	transform = &GetComponent<TransformComponent>();
}

Meteor::~Meteor()
{
}

void Meteor::Init(ComPtr<ID3D11Device>& device)
{
	Object::Init(device);
}

void Meteor::Update(float deltaTime)
{
	Object::Update(deltaTime);

	auto& rotation = transform->Rotation;

	rotation += Vector3(m_angularVelocity.x * deltaTime , m_angularVelocity.y * deltaTime, 0.0f);
	m_updateFlag = true;
}

void Meteor::OnCollisionDetected(Tofino::Collision& collision)
{
	//if (collision.mass != GetComponent<PhysicsComponent>().Mass) return;

	GetComponent<TransformComponent>().Translation -= 5.0f * collision.dt * GetComponent<PhysicsComponent>().Velocity;

	auto& myPhysics = GetComponent<PhysicsComponent>();
	myPhysics.Collider.Translate(GetComponent<TransformComponent>().Translation);

	float totalMass = myPhysics.Mass + collision.mass;

	// Elastic Collision
	// https://en.wikipedia.org/wiki/Elastic_collision
	myPhysics.Velocity =
		(myPhysics.Velocity * (myPhysics.Mass - collision.mass) + collision.velocity * collision.mass * 2.0f) / totalMass;

}
