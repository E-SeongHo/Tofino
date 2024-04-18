#pragma once

#include <vector>
#include <string>

#include "SimpleMath.h"

struct aiScene;
struct aiNode;
struct aiMesh;
namespace Tofino
{
	class Mesh;
	class MeshLoader
	{
	private:
		class ModelReader
		{
		public:
			void ReadModelFile(const std::string& filename, const float scale = 1.0f);
			void ReadNode(aiNode* node, const aiScene* scene, Matrix tr);
			void ReadMesh(aiMesh* mesh, const aiScene* scene);
			//void ReadMaterials(const ::aiScene* scene); //TODO: preload for duplicated textures for optimize

		public: 
			std::vector<Mesh> meshes;
			std::string directory;
		};

	public:
		static std::vector<Mesh> LoadTriangle(const float scale = 1.0f);
		static std::vector<Mesh> LoadSquare(const float scale = 1.0f);
		static std::vector<Mesh> LoadCube(const float scale = 1.0f);
		static std::vector<Mesh> LoadSphere(const float scale = 1.0f);
		static std::vector<Mesh> LoadModel(const std::string& filename, const float scale = 1.0f);
	};

	
}
