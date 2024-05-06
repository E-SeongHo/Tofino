
#include "Scene.h"

#include "Object.h"
#include "Camera.h"
#include "EnvMap.h"
#include "ConstantBuffer.h"
#include "ComponentManager.h"
#include "HashStringManager.h"
#include "InstanceGroup.h"

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
		std::vector<Object*> physicsObjects = Query(ComponentGroup<PhysicsComponent, TransformComponent>{});

		if(m_play)
		{
			// Apply velocity
			for(Object* obj : physicsObjects)
			{
				auto& physicsComponent = physics.Get(obj->GetID());

				Vector3 dv = physicsComponent.Velocity * deltaTime;
				transforms.Get(obj->GetID()).Translation += dv;

				obj->SetUpdateFlag(true);
			}
		}

		for(const auto& obj : m_objects)
		{
			if (obj->IsUpdateFlagSet()) // means either changed transform or texture mapping condition
			{
				auto& transform = transforms.Get(obj->GetID());
				obj->UpdateWorldMatrix(Math::Transformer(transform));
				if(!obj->m_isInstancing) obj->UpdateConstBuffer(RendererContext);

				if(HasComponentOf<PhysicsComponent>(obj->GetID()))
				{
					GetComponentOf<PhysicsComponent>(obj->GetID()).Collider.Scale(transform.Scale);
					GetComponentOf<PhysicsComponent>(obj->GetID()).Collider.Translate(transform.Translation);
					GetComponentOf<PhysicsComponent>(obj->GetID()).Collider.UpdateConstBuffer(RendererContext);
				}
			}
		}

		for(const auto& instanceGroup : m_instanceGroups)
		{
			instanceGroup->UpdateBuffer(RendererContext);
		}

		// Check Collision ( brute force )
		std::vector<CollisionPair> collisionPairs;
		collisionPairs.reserve(MAX_OBJECTS);

		for(auto& p1 : physics)
		{
			for(auto& p2 : physics)
			{
				if(p1.Collider.CheckCollision(p2.Collider))
				{
					collisionPairs.push_back({ &p1, Collision{p2.Velocity, p2.Mass, deltaTime} });
					break;
				}
			}
		}

		for (auto& pair : collisionPairs)
		{
			// some narrow phase check can be added here
			pair.actor->Collider.OnCollisionDetected(pair.collision);
		}
		
		// Meshes (when material has changed)
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
	void Scene::AddComponentOf<MeshComponent>(const ObjectID objID)
	{
		m_componentManager->AddComponent<MeshComponent>(objID);
	}

	template <>
	void Scene::AddComponentOf<PhysicsComponent>(const ObjectID objID)
	{
		assert(HasComponentOf<TransformComponent>(objID));
		assert(HasComponentOf<MeshComponent>(objID)); 

		m_componentManager->AddComponent<PhysicsComponent>(objID);

		// compute min max bound to assign the right fit AABB 
		assert(GetComponentOf<MeshComponent>(objID).Meshes.size()); // considering change it later?

		Vector3 vmin(1000, 1000, 1000);
		Vector3 vmax(-1000, -1000, -1000);
		for (auto& mesh : GetComponentOf<MeshComponent>(objID).Meshes)
		{
			for (auto& v : mesh.GetVertexBuffer().GetData())
			{
				vmin.x = DirectX::XMMin(vmin.x, v.pos.x);
				vmin.y = DirectX::XMMin(vmin.y, v.pos.y);
				vmin.z = DirectX::XMMin(vmin.z, v.pos.z);
				vmax.x = DirectX::XMMax(vmax.x, v.pos.x);
				vmax.y = DirectX::XMMax(vmax.y, v.pos.y);
				vmax.z = DirectX::XMMax(vmax.z, v.pos.z);
			}
		}

		/*std::cout << "lower bound : " << vmin << std::endl;
		std::cout << "upper bound : " << vmax << std::endl;*/

		if (vmin == Vector3(-1.0f) && vmax == Vector3(1.0f)) return;

		GetComponentOf<PhysicsComponent>(objID).Collider = Collider(vmin, vmax);

		// TODO : Register To BVH
	}

	template <>
	MeshComponent& Scene::GetComponentOf<MeshComponent>(const ObjectID objID)
	{
		return m_objectMap[objID]->m_isInstancing ?
			m_objectMap[objID]->m_owningGroup->GetSharingMeshComponent() :
			m_componentManager->GetComponent<MeshComponent>(objID);
	}

	template <>
	bool Scene::HasComponentOf<MeshComponent>(const ObjectID objID)
	{
		return m_objectMap[objID]->m_isInstancing ? 
			true :
			m_componentManager->HasComponent<MeshComponent>(objID);
	}

	void Scene::RegisterObject(Object* obj)
	{
		m_objectMap[obj->GetID()] = obj;
	}
}
