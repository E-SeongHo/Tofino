
#include "SpaceStation.h"
#include "Components.h"
#include "Graphics.h"
#include "MeshLoader.h"

using namespace Tofino;

SpaceStation::SpaceStation(Tofino::Scene* scene, const std::string name, const bool isHittable)
	:Object(scene, name, isHittable)
{
    std::string directory = "D:/Workspace/3DModels/sci-fi-space-station/";

    AddComponent<TransformComponent>();
    AddComponent<MeshComponent>();
    GetComponent<MeshComponent>().Meshes = MeshLoader::LoadModel(directory + "SpaceStation.fbx");

    AddComponent<PhysicsComponent>();
    GetComponent<PhysicsComponent>().Mass = 2000.0f;
    GetComponent<PhysicsComponent>().Collider.BindCollisionEvent(std::bind(&SpaceStation::OnCollisionDetected, this, std::placeholders::_1));

	// Since Assimp failed to find appropriate textures
    {
        Texture bodyAlbedo2(
            RendererDevice, directory + "SpaceStationParts2_BaseColor.png",
            TextureType::ALBEDO);

        GetComponent<MeshComponent>().Meshes[0].GetMaterial().SetAlbedoMap(bodyAlbedo2);
    }

    SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.7f);
    GetComponent<MeshComponent>().Meshes[0].SetMaterialFactors(Vector4(0.0f), 0.1, 0.9f);

    transform = &GetComponent<TransformComponent>();
    transform->Translation = Vector3(-50.0f, 5.0f, 15.0f);
	transform->Scale = Vector3(20.0f);
}

SpaceStation::~SpaceStation()
{
}

void SpaceStation::Init(ComPtr<ID3D11Device>& device)
{
	Object::Init(device);
}

void SpaceStation::Update(float deltaTime)
{
	Object::Update(deltaTime);

    auto& rotation = transform->Rotation;

    rotation += Vector3(0.0f, 0.0f, m_angularAccer * deltaTime);
    m_updateFlag = true;
}

void SpaceStation::OnCollisionDetected(Tofino::Collision& collision)
{
    //GetComponent<TransformComponent>().Translation -= collision.dt * GetComponent<PhysicsComponent>().Velocity;

    //auto& myPhysics = GetComponent<PhysicsComponent>();
    //float totalMass = myPhysics.Mass + collision.mass;

    //// elastic collision
    //myPhysics.Velocity =
    //    (myPhysics.Velocity * (myPhysics.Mass - collision.mass) + collision.velocity * collision.mass * 2.0f) / totalMass;
}
