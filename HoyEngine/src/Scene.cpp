
#include "Scene.h"

#include "Object.h"
#include "Camera.h"
#include "EnvMap.h"
#include "ConstantBuffer.h"
#include "ComponentManager.h"
#include "HashStringManager.h"

namespace Tofino
{
	Scene::Scene()
		:m_constBuffer(ConstantBuffer<GlobalBuffer>(VERTEX_SHADER | GEOMETRY_SHADER | PIXEL_SHADER, 10)),
		 m_componentManager(std::make_unique<ComponentManager>())
	{
	}

	Scene::~Scene()
	{
		for (Light* light : m_lights)
		{
			if (light != nullptr)
			{
				delete light;
				light = nullptr;
			}
		}
	}

	void Scene::Init()
	{
		m_constBuffer.Init(RendererDevice);
	}

	void Scene::AddLight(Light* light)
	{
		m_lights.push_back(light);

		m_constBuffer.GetData().light = *light; // temp
	}

	void Scene::SetMainObject(Object& object)
	{
		// TODO: CHECK IF ITS SCENE STILL OWN THIS OBJECT
		m_mainObject = &object;

		if (m_mainObject->HasComponent<CameraComponent>())
		{
			m_playCamera = &m_mainObject->GetComponent<CameraComponent>().Camera;
		}
	}

	void Scene::AddSkybox(EnvMap* skybox)
	{
		assert(!m_skybox);

		m_skybox.reset(skybox);
		m_skybox->Init(RendererDevice);
	}

	void Scene::Update(float deltaTime) 
	{
		if(m_play)
		{
			for (const auto& obj : m_objects)
			{
				obj->Update(deltaTime);
			}

			if(m_playCamera)
			{
				GlobalBuffer& sceneConst = m_constBuffer.GetData();
				// Updates Scene Buffer every frame
				sceneConst.eye = m_playCamera->GetOrigin();
				sceneConst.view = m_playCamera->GetViewMatrix().Transpose();
				sceneConst.projection = m_playCamera->GetProjectionMatrix().Transpose();
				m_constBuffer.Update(RendererContext);
			}
		}

		// Moves
		auto& physics = m_componentManager->GetContainer<PhysicsComponent>();
		auto& transforms = m_componentManager->GetContainer<TransformComponent>();

		if(m_play)
		{
			std::vector<Object*> physicsObjects = Query(ComponentGroup<PhysicsComponent, TransformComponent>{});
			for(Object* obj : physicsObjects)
			{
				transforms.Get(obj->GetID()).Translation += physics.Get(obj->GetID()).Velocity * deltaTime;
				obj->SetUpdateFlag(true);
			}
		}

		for(const auto& obj : m_objects)
		{
			if (obj->IsUpdateFlagSet()) // means either changed transform or texture mapping condition
			{
				auto transform = transforms.Get(obj->GetID());
				obj->UpdateWorldMatrix(Math::Transformer(transform));
				obj->UpdateConstBuffer(RendererContext);
			}
		}

		// Meshes
		auto& meshes = m_componentManager->GetContainer<MeshComponent>();
		for(auto& meshComponent : meshes)
		{
			auto& meshVector = meshComponent.Meshes;
			for(auto& mesh : meshVector)
			{
				if(mesh.IsUpdateFlagSet())
				{
					mesh.UpdateBuffer(RendererContext);
				}
			}
		}
	}

	void Scene::DestroyObject(Object* object)
	{
		if(object == m_mainObject)
		{
			m_mainObject = nullptr;
		}
	}

	template <>
	void Scene::AddComponentOf<MeshComponent>(const ObjectID objID, const MeshComponent& componentData)
	{
		assert(m_componentManager);

		m_componentManager->AddComponent(objID, std::move(componentData));

		for(auto& mesh : GetComponentOf<MeshComponent>(objID).Meshes)
		{
			mesh.Init(RendererDevice);
		}
	}

	template <>
	void Scene::AddComponentOf<PhysicsComponent>(const ObjectID objID, const PhysicsComponent& componentData)
	{
		assert(m_componentManager);

		m_componentManager->AddComponent(objID, std::move(componentData));

		// 1. add AABB

		// 2. Register To BVH
	}


}
