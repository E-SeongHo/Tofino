
#include "Scene.h"
#include "Graphics.h"

#include <iostream>

Scene::Scene()
	:m_constBuffer(ConstantBuffer<GlobalBuffer>(VERTEX_SHADER | GEOMETRY_SHADER | PIXEL_SHADER, 10))
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

void Scene::BindUpdateFunction(std::function<void(float)> updateFn)
{
	Fn_Update = updateFn;
}

void Scene::SetName(std::string name)
{
	m_name = name;
}

void Scene::SetCamera(Camera* camera)
{
	GlobalBuffer& sceneConst = m_constBuffer.GetData();
	sceneConst.eye = camera->GetOrigin();
	sceneConst.view = camera->GetViewMatrix().Transpose();
	sceneConst.projection = camera->GetProjectionMatrix().Transpose();

	if (m_camera == nullptr)
	{
		m_constBuffer.Init(Graphics::GetDevice());
	}
	else
	{
		m_constBuffer.Update(Graphics::GetDeviceContext());
	}

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
	object->Init(Graphics::GetDevice());
}

void Scene::AddSkybox(EnvMap* skybox)
{
	m_skybox = skybox;
	skybox->Init(Graphics::GetDevice());
}

void Scene::Update(float deltaTime)
{
	if (m_camera == nullptr)
	{
		std::cout << "Scene does not have a Camera" << std::endl;
		return; 
	}

	if(Fn_Update) Fn_Update(deltaTime);

	for (Object* obj : m_objects)
	{
		// obj->Update(deltaTime);
	}

	GlobalBuffer& sceneConst = m_constBuffer.GetData();
	// Updates Scene Buffer every frame
	sceneConst.eye = m_camera->GetOrigin();
	sceneConst.view = m_camera->GetViewMatrix().Transpose();
	sceneConst.projection = m_camera->GetProjectionMatrix().Transpose();
	m_constBuffer.Update(Graphics::GetDeviceContext());

	for (Object* obj : m_objects)
	{
		if (obj->m_updateFlag) obj->UpdateBuffer(Graphics::GetDeviceContext());
	}
}

Camera* Scene::GetCamera()
{
	return m_camera;
}

std::vector<Object*> Scene::GetAllSceneObjects()
{
	return m_objects; // RVO : move semantics
}

EnvMap* Scene::GetSkybox()
{
	return m_skybox;
}

ConstantBuffer<GlobalBuffer>& Scene::GetSceneConstBuffer()
{
	return m_constBuffer;
}
