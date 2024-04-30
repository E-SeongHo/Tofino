#pragma once

#include <Scene.h>
#include "MeteorGenerator.h"

class BattleScene : public Tofino::Scene
{
public:
	BattleScene();

	void Update(float deltaTime) override;

private:
	MeteorGenerator m_meteorGenerator;
};