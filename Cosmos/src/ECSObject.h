#pragma once

#include "Object.h"

class ECSObject : public Tofino::Object
{
public:
	ECSObject(Tofino::Scene* scene, const std::string name = "Untitled", const bool isHittable = true);
	~ECSObject() override {}


private:
	Tofino::TransformComponent* transform;
};
