#pragma once

#include <vector>
#include <functional>
#include <string>

#include "Light.h"
#include "SimpleMath.h"
#include "ConstantBuffer.h"
#include "ComponentManager.h"

namespace Tofino
{
	class Object;
	class Camera;
	class EnvMap;

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

		void Init();

		void BindUpdateFunction(std::function<void(float)> updateFn) { Fn_Update = updateFn; }

		void SetName(const std::string& name) { m_name = name; };

		void SetCamera(Camera* camera);

		void AddLight(Light* light);

		void AddObject(Object* object);

		Object* CreateEmptyObject(const std::string& name);

		void AddSkybox(EnvMap* skybox);

		void Update(float deltaTime);

		Camera* GetCamera() const { return m_camera; }

		std::vector<Object*> GetAllSceneObjects() { return m_objects; }

		EnvMap* GetSkybox() const { return m_skybox; }

		ConstantBuffer<GlobalBuffer>& GetSceneConstBuffer() { return m_constBuffer; }

		template<typename T>
		void AddComponentOf(const ObjectID objID, const T componentData)
		{
			assert(m_componentManager);

			m_componentManager->AddComponent(objID, componentData);
		}

		template<typename T>
		T& GetComponentOf(const ObjectID objID)
		{
			return m_componentManager->GetComponent<T>(objID);
		}

		template<typename T>
		bool HasComponentOf(const ObjectID objID)
		{
			return m_componentManager->HasComponent<T>(objID);
		}


	private:
		friend class Graphics;

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

		std::unique_ptr<ComponentManager> m_componentManager;
	};

}
