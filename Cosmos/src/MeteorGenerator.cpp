
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
		std::string dir = "./Assets/Texture/columned-lava-rock-ue/";
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
		std::string dir = "./Assets/3DModels/the-old-moon/";
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
		std::string dir = "./Assets/3DModels/daphne-planetoid/";
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
		std::string dir = "./Assets/3DModels/asteroid/";
		Texture albedo = Texture(RendererDevice, dir + "base.jpg", TextureType::ALBEDO);

		MeteorData meteorTemp;
		meteorTemp.Name = std::string("asteroid");
		meteorTemp.MeshData = MeshLoader::LoadModel(dir + "asteroid.obj");
		meteorTemp.Textures[0] = albedo;
		m_meteorTemplates.push_back(std::move(meteorTemp));
	}
}

void MeteorGenerator::ShootMeteors(const Vector3& playerLocation)
{
	Vector2 rangeX = Vector2(playerLocation.x - 50.0f, playerLocation.x + 50.0f);
	Vector2 rangeY = Vector2(playerLocation.y - 50.0f, playerLocation.y + 50.0f);

	std::uniform_int_distribution<int> xDistribution(rangeX.x, rangeX.y);
	std::uniform_int_distribution<int> yDistribution(rangeY.x, rangeY.y);

	for (auto instanceGroup : m_meteorGroups)
	{
		for (auto meteor : instanceGroup->GetInstances())
		{
			auto [x, y, z] = m_grid.GetRandomFreeSpace();
			auto& transform = meteor->GetComponent<TransformComponent>();
			transform.Translation = Vector3(x,y, z );
			transform.Scale = Vector3(5.0f);

			Vector2 targetXY = Vector2(xDistribution(s_engine), yDistribution(s_engine));
			Vector2 dirXY = targetXY - Vector2(transform.Translation.x, transform.Translation.y);
			dirXY.Normalize();

			meteor->GetComponent<PhysicsComponent>().Velocity =
				Vector3(dirXY.x * 10.0f, dirXY.y * 10.0f, s_uniformDistribution(s_engine) - 500.0f);
		}
	}

}

void MeteorGenerator::GenerateMeteors(Tofino::Scene* targetScene)
{
	m_meteorPool.reserve(m_maxPool);

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

	ShootMeteors(Vector3(0.0f));
}

