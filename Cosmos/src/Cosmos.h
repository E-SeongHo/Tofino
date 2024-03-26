#pragma once

#include <Application.h>
#include <Scene.h>

class Cosmos : public Application
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
	Scene* m_scene;
};
