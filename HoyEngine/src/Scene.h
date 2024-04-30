#pragma once

#include <vector>
#include <functional>
#include <string>
#include <type_traits>

#include "Light.h"
#include "SimpleMath.h"
#include "ConstantBuffer.h"
#include "ComponentManager.h"
#include "Graphics.h"

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
		void SetName(const std::string& name)							{ m_name = name;		}
		void SetPlay(const bool play)									{ m_play = play;		}
		void SetMainObject(Object& object);

		template<typename T_OBJECT>
		T_OBJECT& CreateObject(const std::string& name)
		{
			static_assert(std::is_base_of<Object, T_OBJECT>::value);
			assert(m_objects.size() <= MAX_OBJECTS);

			m_objects.push_back(std::make_unique<T_OBJECT>(this, name, true));
			m_objects.back()->Init(RendererDevice);

			return *static_cast<T_OBJECT*>(m_objects.back().get());
		}

		void AddLight(Light* light);
		void AddSkybox(EnvMap* skybox);

		virtual void Update(float deltaTime);

		EnvMap& GetSkybox() const										{ return *m_skybox.get();	}
		std::vector<std::unique_ptr<Object>>& GetAllSceneObjects()		{ return m_objects;			}
		ConstantBuffer<GlobalBuffer>& GetSceneConstBuffer()				{ return m_constBuffer;		}
		void DestroyObject(Object* object);

		template<typename T>
		void AddComponentOf(const ObjectID objID, const T& componentData)
		{
			assert(m_componentManager);

			m_componentManager->AddComponent(objID, std::move(componentData));
		}
		template<>
		void AddComponentOf<MeshComponent>(const ObjectID objID, const MeshComponent& componentData);
		template<>
		void AddComponentOf<PhysicsComponent>(const ObjectID objID, const PhysicsComponent& componentData);


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

		template<typename... Components>
		std::vector<Object*> Query(ComponentGroup<Components...>)
		{
			std::vector<IComponentContainer*> containers;
			(containers.push_back(&m_componentManager->GetContainer<Components>()), ...);

			std::vector<Object*> queryObjects;
			for (auto& obj : m_objects)
			{
				bool flag = true;
				for (auto container : containers)
				{
					flag = container->Has(obj->GetID());
					if (!flag) break;
				}
				if (flag) queryObjects.push_back(obj.get());
			}

			return queryObjects;
		}

	protected:
		friend class Graphics;

		std::string m_name;
		bool m_play = false;
		Camera* m_playCamera = nullptr;
		Object* m_mainObject = nullptr; // check object == m_mainObject When destroy object

		std::vector<std::unique_ptr<Object>> m_objects;
		std::vector<Light*> m_lights; // TODO: light will be derived from object soon(to draw emitting)

		// I still don't think it needs to be ref counted, and neither managed with pointer
		// this life scope would be equal with the scene life scope
		// ) If scene deleted : scene will be ref counted, so the member will not be deleted
		ConstantBuffer<GlobalBuffer> m_constBuffer;

		std::unique_ptr<EnvMap> m_skybox = nullptr;

		std::unique_ptr<ComponentManager> m_componentManager;
	};

}
