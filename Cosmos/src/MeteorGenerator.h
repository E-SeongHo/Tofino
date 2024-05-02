#pragma once

#include <vector>

namespace Tofino
{
	class Scene;
	class Mesh;
	class Texture;
}

class Meteor;

struct MeteorData
{
	std::vector<Tofino::Mesh> MeshData;
	Tofino::Texture Textures[3]; // [0] : albedo, [1] : height, [2] : normal
};

class MeteorGenerator
{
public:
	MeteorGenerator();
	~MeteorGenerator() = default;

	void ShootMeteors();
	void GenerateMeteors(Tofino::Scene* targetScene);

private:
	std::vector<MeteorData> m_meteorTemplates;
	std::vector<Meteor*> m_meteorPool;

	const int m_maxPool = 1000;
};