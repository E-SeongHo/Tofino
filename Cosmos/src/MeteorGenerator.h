#pragma once

#include <vector>
#include <unordered_set>

namespace Tofino
{
	class Scene;
	class Mesh;
	class Texture;
	class InstanceGroup;
}

class Meteor;

struct MeteorData
{
	std::string Name;
	std::vector<Tofino::Mesh> MeshData;
	Tofino::Texture Textures[3]; // [0] : albedo, [1] : height, [2] : normal
};

class Grid
{
	using GridIndex = std::tuple<int, int, int>;
	using GridCoords = std::tuple<int, int, int>;

public:
	Grid();
	~Grid() = default;

	GridIndex GetRandomFreeSpace()
	{
		GridIndex index = m_availableCells.back();
		m_availableCells.pop_back();

		return IndexToCoords(index);
	}

	GridCoords IndexToCoords(const GridIndex& xyz)
	{
		auto& [x, y, z] = xyz;
		return std::make_tuple(x * m_cellSize - 250, y * m_cellSize - 250, z * m_cellSize);
	}

private:
	std::vector<GridIndex> m_availableCells;

	int m_spaceWidth = 500; //  [-500, 500]
	int m_spaceHeight = 500; // [-500, 500]
	int m_spaceDepth = 200; // [0, 200]

	int m_cellSize = 10; // 100 x 100 x 20 = 200000
};

class MeteorGenerator
{
public:
	MeteorGenerator();
	~MeteorGenerator() = default;

	void ShootMeteors(const Tofino::Vector3& playerLocation);
	void GenerateMeteors(Tofino::Scene* targetScene);

private:
	std::vector<MeteorData> m_meteorTemplates;
	std::vector<Meteor*> m_meteorPool;
	std::vector<Tofino::InstanceGroup*> m_meteorGroups;

	const int m_maxPool = 2000;
	Grid m_grid;
};