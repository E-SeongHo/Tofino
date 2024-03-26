
#include "Scene.h"
#include "Graphics.h"


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
	Fn_Update= updateFn;
}

void Scene::SetName(std::string name)
{
	m_name = name;
}

void Scene::SetCamera(Camera* camera)
{
	m_camera = camera;
}

void Scene::AddLight(Light* light)
{
	m_lights.push_back(light);
}

void Scene::AddObject(Object* object)
{
	m_objects.push_back(object);
	RendererInstance.InitObject(object);
}

void Scene::AddSkybox(EnvMap* skybox)
{
	m_skybox = skybox;
	RendererInstance.InitSkybox(skybox);
}

void Scene::AddSceneBuffer(GlobalBuffer* sceneConst)
{
	m_sceneConstBufferCPU = sceneConst;
	RendererInstance.InitSceneGlobal(sceneConst);
}

void Scene::Update(float deltaTime)
{
	if(Fn_Update) Fn_Update(deltaTime);

	for (Object* obj : m_objects)
	{
		// obj->Update(deltaTime);
	}

	m_sceneConstBufferCPU->eye = m_camera->GetOrigin();
	m_sceneConstBufferCPU->view = m_camera->GetViewMatrix().Transpose();
	m_sceneConstBufferCPU->projection = m_camera->GetProjectionMatrix().Transpose();
	RendererInstance.UploadGlobalConst(m_sceneConstBufferCPU);

	for (Object* obj : m_objects)
	{
		if (obj->m_updateFlag) obj->UpdateBuffer(RendererInstance.GetDeviceContext());
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

GlobalBuffer* Scene::GetSceneBuffer()
{
	return m_sceneConstBufferCPU;
}
