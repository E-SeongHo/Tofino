#pragma once

#include "Object.h"
#include "Camera.h"
#include "EnvMap.h"
#include "Light.h"

#include <vector>
#include <functional>
#include <string>

// Global for scene
struct GlobalBuffer
{   // Make sure must be a column matrix
	DirectX::SimpleMath::Matrix view;
	DirectX::SimpleMath::Matrix projection;

	DirectX::SimpleMath::Vector3 eye; // world
	float padding1;
	Light light; // 32bytes
};

class Scene
{
public:
	Scene() = default;
	~Scene();
	
	// Binds an update function of the scene
	void BindUpdateFunction(std::function<void(float)> updateFn);

	// Sets a name of the scene
	void SetName(std::string name);

	// Sets a camera for the scene
	void SetCamera(Camera* camera);

	// Adds a light in the scene
	void AddLight(Light* light);

	// Adds object on the scene and init object
	void AddObject(Object* object);

	// Adds skybox on the scene and init object
	void AddSkybox(EnvMap* skybox);

	// Adds scene const buffer
	void AddSceneBuffer(GlobalBuffer* sceneConst);

	// Update : Calls bound update function
	void Update(float deltaTime);

	// Returns camera used in the scene
	Camera* GetCamera();

	// Returns vector of all objects in the scene
	std::vector<Object*> GetAllSceneObjects();

	// Returns skybox of the scene
	EnvMap* GetSkybox();

	// Returns scene const buffer reference
	GlobalBuffer* GetSceneBuffer();

private:
	// Update Function for the scene
	std::function<void(float dt)> Fn_Update;

	std::string m_name;

	Camera* m_camera = nullptr;
	std::vector<Object*> m_objects;
	std::vector<Light*> m_lights; // TODO: light will be derived from object soon(to draw emitting)

	GlobalBuffer* m_sceneConstBufferCPU;

	EnvMap* m_skybox = nullptr;
};

