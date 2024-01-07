#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

#include "Mesh.h"

class Model
{
public:
	void LoadModel(const std::string& filename);
	void Init(ComPtr<ID3D11Device>& device);
	void Render(ComPtr<ID3D11DeviceContext>& context);
	void UpdateWorldMatrix(DirectX::SimpleMath::Matrix worldRow);
	void UpdateBuffer(ComPtr<ID3D11DeviceContext>& context);
	DirectX::SimpleMath::Matrix GetWorldMatrix();

private:
	void LoadNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	//void LoadMaterials(const aiScene* scene); // texture가 많아지면 optimize : preload for duplicated textures

private:
	std::string m_directory;
	std::vector<Mesh> m_meshes;

	ModelBuffer m_constBufferCPU;
	ComPtr<ID3D11Buffer> m_constBufferGPU;
};