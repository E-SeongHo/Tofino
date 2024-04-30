#pragma once

#include <Application.h>
#include <vector>

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
	std::vector<std::unique_ptr<Tofino::Scene>> m_scenes;
};
