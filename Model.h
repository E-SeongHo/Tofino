#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

#include "Mesh.h"
#include "Material.h"

class Model : public Hittable
{
public:
	void LoadModel(const std::string& filename);
	void Init(ComPtr<ID3D11Device>& device, const float scale, const bool isHittable);
	void Render(ComPtr<ID3D11DeviceContext>& context);
	void RenderNormal(ComPtr<ID3D11DeviceContext>& context);
	void UpdateWorldMatrix(DirectX::SimpleMath::Matrix worldRow);
	void UpdateBuffer(ComPtr<ID3D11DeviceContext>& context);
	DirectX::SimpleMath::Matrix GetWorldMatrix();

private:
	void LoadNode(aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	//void LoadMaterials(const aiScene* scene); // texture가 많아지면 optimize : preload for duplicated textures

private:
	ComPtr<ID3D11Buffer> m_modelBufferGPU;

public:
	ModelBuffer m_modelBufferCPU;
	
	std::string m_directory;
	std::vector<Mesh> m_meshes;
};