
#include "Aircraft.h"
#include "Components.h"
#include "Graphics.h"
#include "Input.h"
#include "MeshLoader.h"

using namespace Tofino;

Aircraft::Aircraft(Tofino::Scene* scene, const std::string name, const bool isHittable)
	:Object(scene, name, isHittable)
{
	AddComponent<TransformComponent>(TransformComponent());
    std::string directory = "D:/Workspace/3DModels/E-45-Aircraft/";

    AddComponent<MeshComponent>(MeshComponent(
        { MeshLoader::LoadModel(directory + "E 45 Aircraft_obj.obj") }
    ));

    // Since Assimp failed to find appropriate textures
    {
        Texture bodyAlbedo(RendererDevice, directory + "E-45 _col.jpg", TextureType::ALBEDO);
        Texture bodyNormal(RendererDevice, directory + "E-45-nor_1.jpg", TextureType::NORMAL);

        Texture glassNormal(RendererDevice, directory + "E-45_glass_nor_.jpg", TextureType::NORMAL);

        // body
        GetComponent<MeshComponent>().Meshes[0].GetMaterial().SetAlbedoMap(bodyAlbedo);
        GetComponent<MeshComponent>().Meshes[0].GetMaterial().SetNormalMap(bodyNormal);
        GetComponent<MeshComponent>().Meshes[0].GetMaterial().GetMaterialStatus().hasHeightMap = 0;

        // glass
        GetComponent<MeshComponent>().Meshes[1].GetMaterial().GetMaterialStatus().hasAlbedoMap = 0;
        GetComponent<MeshComponent>().Meshes[1].GetMaterial().SetNormalMap(glassNormal);
        GetComponent<MeshComponent>().Meshes[1].GetMaterial().GetMaterialStatus().hasHeightMap = 0;
    }

    SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.7f);
    transform = &GetComponent<TransformComponent>();

    transform->Translation = Vector3(7.0f, 10.0f, -150.0f);
    transform->Scale = Vector3(15.0f);

    AddComponent<CameraComponent>({ Camera() });
    
    camera = &GetComponent<CameraComponent>();

    // Sets offset of camera component
    camera->Camera.SetOrigin(transform->Translation + cameraOffset);
}

Aircraft::~Aircraft()
{

}

void Aircraft::Init(ComPtr<ID3D11Device>& device)
{
	Object::Init(device);

}

void Aircraft::Update(float deltaTime)
{
	Object::Update(deltaTime);

    constexpr Vector3 forwardDir = Vector3(0.0f, 0.0f, 1.0f);
    constexpr Vector3 rightDir = Vector3(1.0f, 0.0f, 0.0f);

    auto& location = transform->Translation;
    auto& rotation = transform->Rotation;

	if (Input::IsKeyPressed(VK_UP))
    {
        location += forwardDir * m_speed * deltaTime;
        m_updateFlag = true;
    }

    if (Input::IsKeyPressed(VK_DOWN))
    {
        location -= forwardDir * m_speed * deltaTime;
        m_updateFlag = true;
    }

    if(Input::IsKeyPressed(VK_LEFT))
    {
        location -= rightDir * m_speed * deltaTime;
        rotation.z += 10.0f * deltaTime;
        m_updateFlag = true;
    }

    if(Input::IsKeyPressed(VK_RIGHT))
    {
        location += rightDir * m_speed * deltaTime; 
        rotation.z -= 10.0f * deltaTime;
        m_updateFlag = true;
    }

    camera->Camera.SetOrigin(location + cameraOffset);
}
