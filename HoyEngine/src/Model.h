#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

#include "Object.h"

namespace Tofino
{
	class Model : public Object
	{
	public:
		Model(const std::string name = "Untitled", const bool isHittable = true);

		void LoadModel(const std::string& filename, const float scale = 1.0f);

	private:
		void LoadNode(aiNode* node, const aiScene* scene, Matrix tr);
		void LoadMesh(aiMesh* mesh, const aiScene* scene);
		//void LoadMaterials(const aiScene* scene); // TODO: preload for duplicated textures for optimize

	public:
		std::string m_directory;
	};
}