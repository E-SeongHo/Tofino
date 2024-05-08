
#include <MeshLoader.h>
#include <random>

#include <Scene.h>
#include <InstanceGroup.h>

#include "MeteorGenerator.h"
#include "Meteor.h"

using namespace Tofino;

static std::random_device s_randomDevice;
static std::mt19937_64 s_engine(s_randomDevice());
static std::uniform_int_distribution<uint32_t> s_uniformDistribution(0, 500);

Grid::Grid()
{
	int numCellsX = (m_spaceWidth) / (m_cellSize);
	int numCellsY = (m_spaceHeight) / (m_cellSize);
	int numCellsZ = (m_spaceDepth) / (m_cellSize);

	m_availableCells.reserve(numCellsX * numCellsY * numCellsZ);

	// 100 x 100 x 20 = 200000
	for(int z = 0; z < numCellsZ; z++)
	{
		for(int y = 0; y < numCellsY; y++)
		{
			for(int x = 0; x < numCellsX; x++)
			{
				m_availableCells.push_back(std::make_tuple(x, y, z));
			}
		}
	}

	std::shuffle(m_availableCells.begin(), m_availableCells.end(), s_engine);
}

MeteorGenerator::MeteorGenerator()
{
	{
		std::string dir = "D:/Workspace/HoyEngine/Cosmos/Assets/Texture/columned-lava-rock-ue/";
		Texture albedo = Texture(RendererDevice, dir + "columned-lava-rock_albedo.png", TextureType::ALBEDO);
		Texture height = Texture(RendererDevice, dir + "columned-lava-rock_height.png", TextureType::HEIGHT);
		Texture normal = Texture(RendererDevice, dir + "columned-lava-rock_normal-dx.png", TextureType::NORMAL);

		MeteorData meteorTemp;
		meteorTemp.Name = std::string("LavaRock");
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
		meteorTemp.Name = std::string("old-moon");
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
		meteorTemp.Name = std::string("planetoid");
		meteorTemp.MeshData = MeshLoader::LoadModel(dir + "Daphne_LP.fbx");
		meteorTemp.Textures[0] = albedo;
		meteorTemp.Textures[2] = normal;
		m_meteorTemplates.push_back(std::move(meteorTemp));
	}

	{
		std::string dir = "D:/Workspace/HoyEngine/Cosmos/Assets/3DModels/asteroid/";
		Texture albedo = Texture(RendererDevice, dir + "base.jpg", TextureType::ALBEDO);

		MeteorData meteorTemp;
		meteorTemp.Name = std::string("asteroid");
		meteorTemp.MeshData = MeshLoader::LoadModel(dir + "asteroid.obj");
		meteorTemp.Textures[0] = albedo;
		m_meteorTemplates.push_back(std::move(meteorTemp));
	}
}

void MeteorGenerator::ShootMeteors()
{
	/*for(auto& meteor : m_meteorPool)
	{
		auto [x, y, z] = m_grid.GetRandomFreeSpace();
		meteor->GetComponent<TransformComponent>().Translation = Vector3(x, y, z);
		meteor->GetComponent<PhysicsComponent>().Velocity =
			Vector3(s_uniformDistribution(s_engine) * 0.1f, s_uniformDistribution(s_engine) * 0.1f, s_uniformDistribution(s_engine) - 500.0f);
	}*/

	for (auto instanceGroup : m_meteorGroups)
	{
		for (auto meteor : instanceGroup->GetInstances())
		{
			auto [x, y, z] = m_grid.GetRandomFreeSpace();
			meteor->GetComponent<TransformComponent>().Translation = Vector3(x,y, z );
			meteor->GetComponent<TransformComponent>().Scale = Vector3(5.0f);
			meteor->GetComponent<PhysicsComponent>().Velocity =
				Vector3(s_uniformDistribution(s_engine) * 0.1f, s_uniformDistribution(s_engine) * 0.1f, s_uniformDistribution(s_engine) - 500.0f);
		}
	}

}

void MeteorGenerator::GenerateMeteors(Tofino::Scene* targetScene)
{
	m_meteorPool.reserve(m_maxPool);

	//{
	//	const auto& meteorData = m_meteorTemplates[1];
	//	Meteor& meteor = targetScene->CreateObject<Meteor>("meteor-" + std::to_string(m_meteorPool.size()));
	//	meteor.GetComponent<TransformComponent>().Translation =
	//		Vector3(-20.0f, 0.0f, 50.0f);
	//	meteor.GetComponent<TransformComponent>().Scale = Vector3(5.0f);

	//	// copy template's data, but it only reads the file once
	//	meteor.AddComponent<MeshComponent>();
	//	meteor.GetComponent<MeshComponent>().Meshes = meteorData.MeshData;

	//	// hack : because only one of the templates have 2 meshes and need texture for the second mesh
	//	meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetAlbedoMap(meteorData.Textures[0]);
	//	meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetHeightMap(meteorData.Textures[1]);
	//	meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetNormalMap(meteorData.Textures[2]);
	//	meteor.SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.1f);

	//	meteor.AddComponent<PhysicsComponent>();
	//	meteor.GetComponent<PhysicsComponent>().Velocity =
	//		Vector3(10.0f, 0.0f, -5.0f);
	//	meteor.GetComponent<PhysicsComponent>().Collider.BindCollisionEvent(
	//		std::bind(&Meteor::OnCollisionDetected, &meteor, std::placeholders::_1));
	//}

	//{
	//	const auto& meteorData = m_meteorTemplates[1];
	//	Meteor& meteor = targetScene->CreateObject<Meteor>("meteor-" + std::to_string(m_meteorPool.size()));
	//	meteor.GetComponent<TransformComponent>().Translation =
	//		Vector3(20.0f, 0.0f, 50.0f);
	//	meteor.GetComponent<TransformComponent>().Scale = Vector3(5.0f);

	//	// copy template's data, but it only reads the file once
	//	meteor.AddComponent<MeshComponent>();
	//	meteor.GetComponent<MeshComponent>().Meshes = meteorData.MeshData;

	//	// hack : because only one of the templates have 2 meshes and need texture for the second mesh
	//	meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetAlbedoMap(meteorData.Textures[0]);
	//	meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetHeightMap(meteorData.Textures[1]);
	//	meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetNormalMap(meteorData.Textures[2]);
	//	meteor.SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.1f);

	//	meteor.AddComponent<PhysicsComponent>();
	//	meteor.GetComponent<PhysicsComponent>().Velocity =
	//		Vector3(0.0f, 0.0f, -5.0f);
	//	meteor.GetComponent<PhysicsComponent>().Collider.BindCollisionEvent(
	//		std::bind(&Meteor::OnCollisionDetected, &meteor, std::placeholders::_1));
	//}

	// Create Instances 
	for(auto& data : m_meteorTemplates)
	{
		MeshComponent meshComp({ data.MeshData });
		meshComp.Meshes.back().GetMaterial().SetAlbedoMap(data.Textures[0]);
		meshComp.Meshes.back().GetMaterial().SetHeightMap(data.Textures[1]);
		meshComp.Meshes.back().GetMaterial().SetNormalMap(data.Textures[2]);

		for(auto& mesh : meshComp.Meshes)
		{
			mesh.SetMaterialFactors(Vector4(0.0f), 0.1f, 0.1f);
			//mesh.UpdateBuffer(RendererContext);
		}

		InstanceGroup* ig = &targetScene->CreateObjectInstances<Meteor>(meshComp, data.Name, m_maxPool / m_meteorTemplates.size());
		m_meteorGroups.push_back(ig);
	}

	// Add Components
	for(auto instanceGroup : m_meteorGroups)
	{
		for(auto meteorObj : instanceGroup->GetInstances())
		{
			Meteor* meteor = static_cast<Meteor*>(meteorObj);
			meteor->AddComponent<PhysicsComponent>();

			Vector3 angularVelocity = Vector3(s_uniformDistribution(s_engine) * 0.25f, s_uniformDistribution(s_engine) * 0.25f, s_uniformDistribution(s_engine) * 0.25f);
			meteor->SetAngularVelocity(angularVelocity);
		}
	}

	ShootMeteors();

	//for(int i = 0; i < m_maxPool; i++)
	//{
	//	Meteor& meteor = targetScene->CreateObject<Meteor>("meteor-" + std::to_string(m_meteorPool.size()));

	//	const int index = s_uniformDistribution(s_engine) % 4;
	//	const auto& meteorData = m_meteorTemplates[index];

	//	auto [x, y, z] = m_grid.GetRandomFreeSpace();
	//	meteor.GetComponent<TransformComponent>().Translation = Vector3(x, y, z);
	//	meteor.GetComponent<TransformComponent>().Scale = Vector3(5.0f);

	//	// copy template's data, but it only reads the file once
	//	meteor.AddComponent<MeshComponent>();
	//	meteor.GetComponent<MeshComponent>().Meshes = meteorData.MeshData;

	//	// hack : because only one of the templates have 2 meshes and need texture for the second mesh
	//	meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetAlbedoMap(meteorData.Textures[0]);
	//	meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetHeightMap(meteorData.Textures[1]);
	//	meteor.GetComponent<MeshComponent>().Meshes.back().GetMaterial().SetNormalMap(meteorData.Textures[2]);
	//	meteor.SetAllMeshMaterialFactors(Vector4(0.0f), 0.1f, 0.1f);

	//	meteor.AddComponent<PhysicsComponent>();
	//	meteor.GetComponent<PhysicsComponent>().Velocity =
	//		Vector3(s_uniformDistribution(s_engine) * 0.1f, s_uniformDistribution(s_engine) * 0.1f, s_uniformDistribution(s_engine) - 500.0f);
	//	meteor.GetComponent<PhysicsComponent>().Mass = 50.0f;
	//	meteor.GetComponent<PhysicsComponent>().Collider.BindCollisionEvent(
	//		std::bind(&Meteor::OnCollisionDetected, &meteor, std::placeholders::_1));

	//	Vector3 angularVelocity = Vector3(s_uniformDistribution(s_engine) * 0.25f, s_uniformDistribution(s_engine) * 0.25f, s_uniformDistribution(s_engine) * 0.25f);
	//	meteor.SetAngularVelocity(angularVelocity);
	//	 
	//	m_meteorPool.push_back(&meteor);
	//}
}

