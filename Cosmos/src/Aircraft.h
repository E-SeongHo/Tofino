#pragma once

#include <Object.h>

class Tofino::Scene;

class Aircraft : public Tofino::Object
{
public:
	Aircraft(Tofino::Scene* scene, const std::string name = "Untitled", const bool isHittable = true);
	~Aircraft() override;

	// Called once when scene init
	void Init(ComPtr<ID3D11Device>& device) override;

	// Called every frame
	void Update(float deltaTime) override;

	void OnCollisionDetected(Tofino::Collision& collision);

private:
	float m_speed = 15.0f;
	Tofino::Vector3 cameraOffset = Tofino::Vector3(4.0f, 5.0f, -15.0f);

	Tofino::TransformComponent* transform;
	Tofino::CameraComponent* camera;
};

