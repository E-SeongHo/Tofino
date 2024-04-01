#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

#include "Object.h"


class Model : public Object
{
public:
	Model(const std::string name = "Untitled", const bool isHittable = true);

	void LoadModel(const std::string& filename, const float scale = 1.0f);

private:
	void NormalizeVertices(const float scale);
	void LoadNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	//void LoadMaterials(const aiScene* scene); // texture가 많아지면 optimize : preload for duplicated textures

public:	
	std::string m_directory;
};