#pragma once

#include "Model.h"
#include "Camera.h"
#include "EnvMap.h"
#include "Light.h"

#include <vector>
#include <functional>
#include <string>

namespace Tofino
{
	// Global for scene
	struct GlobalBuffer
	{   // Make sure must be a column matrix
		Matrix view;
		Matrix projection;

		Vector3 eye; // world
		float padding1;
		Light light; // 32bytes
	};

	class Scene
	{
	public:
		Scene();
		~Scene();

		void BindUpdateFunction(std::function<void(float)> updateFn);

		void SetName(std::string name);

		void SetCamera(Camera* camera);

		void AddLight(Light* light);

		void AddObject(Object* object);

		void AddSkybox(EnvMap* skybox);

		void Update(float deltaTime);

		Camera* GetCamera();

		std::vector<Object*> GetAllSceneObjects();

		EnvMap* GetSkybox();

		ConstantBuffer<GlobalBuffer>& GetSceneConstBuffer();

	private:
		std::function<void(float dt)> Fn_Update;

		std::string m_name;

		Camera* m_camera = nullptr;
		std::vector<Object*> m_objects;
		std::vector<Light*> m_lights; // TODO: light will be derived from object soon(to draw emitting)

		// I still don't think it needs to be ref counted, and neither managed with pointer
		// this life scope would be equal with the scene life scope
		// ) If scene deleted : scene will be ref counted, so the member will not be deleted
		ConstantBuffer<GlobalBuffer> m_constBuffer;

		EnvMap* m_skybox = nullptr;
	};

}