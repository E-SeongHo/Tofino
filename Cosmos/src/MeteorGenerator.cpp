
#include <MeshLoader.h>
#include <random>

#include <Scene.h>

#include "MeteorGenerator.h"
#include "Meteor.h"

using namespace Tofino;

static std::random_device s_randomDevice;
static std::mt19937_64 s_engine(s_randomDevice());
static std::uniform_int_distribution<uint32_t> s_uniformDistribution(0, 200);

MeteorGenerator::MeteorGenerator()
{
	{
		std::string dir = "D:/Workspace/HoyEngine/Cosmos/Assets/Texture/columned-lava-rock-ue/";
		Texture albedo = Texture(RendererDevice, dir + "columned-lava-rock_albedo.png", TextureType::ALBEDO);
		Texture height = Texture(RendererDevice, dir + "columned-lava-rock_height.png", TextureType::HEIGHT);
		Texture normal = Texture(RendererDevice, dir + "columned-lava-rock_normal-dx.png", TextureType::NORMAL);

		MeteorData meteorTemp;
		meteorTemp.MeshData = MeshLoader::LoadSphere();
		meteorTemp.Textures[0] = albedo;
		meteorTemp.Textures[1] = height;
		meteorTemp.Textures[2] = normal;
		m_meteorTemplates.push_back(std::move(meteorTemp));
	}

	{
		std::string dir = "D:/Workspace/HoyEngine/Cosmos/Assets/3DModels/the-old-moon/";
		Texture albedo = Texture(RendererDevice, dir + "1_Base_color.jpg", TextureType::ALBEDO);
		Texture normal = Texture(RendererDevice, dir + "1_Normal_DirectX.jpg", TextureType::NORMAL);

		MeteorData meteorTemp;
		meteorTemp.MeshData = MeshLoader::LoadModel(dir + "OldMoon_LP.fbx");
		meteorTemp.Textures[0] = albedo;
		meteorTemp.Textures[2] = normal;
		m_meteorTemplates.push_back(std::move(meteorTemp));
	}

	{
		std::string dir = "D:/Workspace/HoyEngine/Cosmos/Assets/3DModels/daphne-planetoid/";
		Texture albedo = Texture(RendererDevice, dir + "1_Base_color.jpg", TextureType::ALBEDO);
		Texture normal = Texture(RendererDevice, dir + "1_Normal_DirectX.jpg", TextureType::NORMAL);

		MeteorData meteorTemp;
		meteorTemp.MeshData = MeshLoader::LoadModel(dir + "Daphne_LP.fbx");
		meteorTemp.Textures[0] = albedo;
		meteorTemp.Textures[2] = normal;
		m_meteorTemplates.push_back(std::move(meteorTemp));
	}

	{
		std::string dir = "D:/Workspace/HoyEngine/Cosmos/Assets/3DModels/asteroid/";
		Texture albedo = Texture(RendererDevice, dir + "base.jpg", TextureType::ALBEDO);

		MeteorData meteorTemp;
		meteorTemp.MeshData = MeshLoader::LoadModel(dir + "asteroid.obj");
		meteorTemp.Textures[0] = albedo;
		m_meteorTemplates.push_back(std::move(meteorTemp));
	}
}

void MeteorGenerator::ShootMeteors()
{
	for(auto& meteor : m_meteorPool)
	{
		meteor->GetComponent<TransformComponent>().Translation =
			Vector3(s_uniformDistribution(s_engine) - 100.0f, s_uniformDistribution(s_engine) - 100.0f, 100.0f);
	}
}

void MeteorGenerator::GenerateMeteors(Tofino::Scene* targetScene)
{
	m_meteorPool.reserve(m_maxPool);

	for(int i = 0; i < m_maxPool; i++)
	{
		Meteor& meteor = targetScene->CreateObject<Meteor>("meteor-" + std::to_string(m_meteorPool.size()));

		const int index = s_uniformDistribution(s_engine) % 4;
		const auto& meteorData = m_meteorTemplates[index];

		meteor.GetComponent<TransformComponent>().Translation =
			Vector3(s_uniformDistribution(s_engine) - 100.0f, s_uniformDistribution(s_engine) - 100.0f, 100.0f);
		meteor.GetComponent<TransformComponent>().Scale = Vector3(5.0f);

		// copy template's data, but it only reads the file once
		meteor.AddComponent<MeshComponent>();
		meteor.GetComponent<MeshComponent>().Meshes = meteorData.MeshData;

		// hack : because only one of the templates have 2 meshes and need texture for the second mesh
		meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetAlbedoMap(meteorData.Textures[0]);
		meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetHeightMap(meteorData.Textures[1]);
		meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetNormalMap(meteorData.Textures[2]);
		meteor.SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.1f);

		meteor.AddComponent<PhysicsComponent>();
		meteor.GetComponent<PhysicsComponent>().Velocity =
			Vector3(s_uniformDistribution(s_engine) * 0.1f, s_uniformDistribution(s_engine) * 0.1f, s_uniformDistribution(s_engine) - 100.0f);

		Vector3 angularVelocity = Vector3(s_uniformDistribution(s_engine) * 0.25f, s_uniformDistribution(s_engine) * 0.25f, s_uniformDistribution(s_engine) * 0.25f);
		meteor.SetAngularVelocity(angularVelocity);

		m_meteorPool.push_back(&meteor);
	}
}

