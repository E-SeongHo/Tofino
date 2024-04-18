
#include "Scene.h"

#include "Graphics.h"
#include "Object.h"
#include "Camera.h"
#include "EnvMap.h"
#include "ConstantBuffer.h"
#include "ComponentManager.h"

namespace Tofino
{
	Scene::Scene()
		: m_constBuffer(ConstantBuffer<GlobalBuffer>(VERTEX_SHADER | GEOMETRY_SHADER | PIXEL_SHADER, 10)),
		  m_componentManager(std::make_unique<ComponentManager>())
	{
	}

	Scene::~Scene()
	{
		for (Object* obj : m_objects)
		{
			if (obj != nullptr)
			{
				delete obj;
				obj = nullptr;
			}
		}

		for (Light* light : m_lights)
		{
			if (light != nullptr)
			{
				delete light;
				light = nullptr;
			}
		}

		if (m_skybox != nullptr)
		{
			delete m_skybox;
			m_skybox = nullptr;
		}

		if (m_camera != nullptr)
		{
			delete m_camera;
			m_camera = nullptr;
		}
	}

	void Scene::Init()
	{
		assert(m_camera);

		m_constBuffer.Init(RendererDevice);

		auto& transforms = m_componentManager->GetContainer<TransformComponent>();
		for(Object* obj : m_objects)
		{
			obj->Init(RendererDevice);

			auto transform = transforms.Get(obj->GetID());
			obj->UpdateWorldMatrix(Math::Transformer(transform));
			obj->UpdateConstBuffer(RendererContext);
		}

		for (auto& meshComponent : m_componentManager->GetContainer<MeshComponent>())
		{
			for (auto& mesh : meshComponent.Meshes)
			{
				mesh.Init(RendererDevice);
			}
		}

		if (m_skybox)
		{
			m_skybox->Init(RendererDevice);
		}
	}

	void Scene::SetCamera(Camera* camera)
	{
		GlobalBuffer& sceneConst = m_constBuffer.GetData();
		sceneConst.eye = camera->GetOrigin();
		sceneConst.view = camera->GetViewMatrix().Transpose();
		sceneConst.projection = camera->GetProjectionMatrix().Transpose();

		// Change the render camera while rendering
		if(m_camera) m_constBuffer.Update(RendererContext);

		m_camera = camera;
	}

	void Scene::AddLight(Light* light)
	{
		m_lights.push_back(light);

		m_constBuffer.GetData().light = *light; // temp
	}

	void Scene::AddObject(Object* object)
	{
		m_objects.push_back(object);
		object->Init(RendererDevice);
	}

	Object* Scene::CreateEmptyObject(const std::string& name)
	{
		Object* obj = new Object(this, name, true);

		m_objects.push_back(obj);
		return obj;
	}

	void Scene::AddSkybox(EnvMap* skybox)
	{
		assert(!m_skybox);

		m_skybox = skybox;
		// skybox->Init(RendererDevice);
	}

	void Scene::Update(float deltaTime)
	{
		assert(m_camera);

		if (Fn_Update) Fn_Update(deltaTime);

		for (Object* obj : m_objects)
		{
			// obj->Update(deltaTime);
		}

		GlobalBuffer& sceneConst = m_constBuffer.GetData();
		// Updates Scene Buffer every frame
		sceneConst.eye = m_camera->GetOrigin();
		sceneConst.view = m_camera->GetViewMatrix().Transpose();
		sceneConst.projection = m_camera->GetProjectionMatrix().Transpose();
		m_constBuffer.Update(RendererContext);

		// Process Changed data from GUI 
		auto& transforms = m_componentManager->GetContainer<TransformComponent>();
		for(Object* obj : m_objects)
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

		/*for (Object* obj : m_objects)
		{
			if (obj->IsUpdateFlagSet()) obj->UpdateBuffer(RendererContext);
		}*/
	}
}