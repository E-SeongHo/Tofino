#pragma once

#include <Object.h>

class SpaceStation : public Tofino::Object
{
public:
	SpaceStation(Tofino::Scene* scene, const std::string name = "Untitled", const bool isHittable = true);
	~SpaceStation() override;

	// Called once when scene init
	void Init(ComPtr<ID3D11Device>& device) override;

	// Called every frame
	void Update(float deltaTime) override;

private:
	float m_angularAccer = 30.0f;

	Tofino::TransformComponent* transform;
};

