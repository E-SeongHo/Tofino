#pragma once

#include "Object.h"

class Meteor : public Tofino::Object
{
public:
	Meteor(Tofino::Scene* scene, const std::string name = "Untitled", const bool isHittable = true);
	~Meteor() override;

	// Called once when scene init
	void Init(ComPtr<ID3D11Device>& device) override;

	// Called every frame
	void Update(float deltaTime) override;

	void SetAngularVelocity(const Tofino::Vector3& velocity) { m_angularVelocity = velocity; }

	void OnCollisionDetected(Tofino::Collision& collision) override;

private:
	Tofino::TransformComponent* transform;
	Tofino::Vector3 m_angularVelocity;
};

