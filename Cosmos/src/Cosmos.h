#pragma once

#include <Application.h>

class Tofino::Scene;

class Cosmos : public Tofino::Application
{
public:
	Cosmos();
	~Cosmos() override;

	bool Init() override;

	void Update(float deltaTime) override;

	void RenderGUI() override;

private:
	void DesignScenes();

private:
	Tofino::Scene* m_scene;
};
