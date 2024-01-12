#pragma once

#include <string>
#include <vector>

#include "Mesh.h"
#include "Material.h"

using Microsoft::WRL::ComPtr;

class Hittable
{
public:
	bool hitEnabled = false;
	DirectX::BoundingSphere m_boundingSphere;
};

class Object : public Hittable
{
public:
	void Init(ComPtr<ID3D11Device>& device, const bool isHittable);
	void Render(ComPtr<ID3D11DeviceContext>& context);
	void RenderNormal(ComPtr<ID3D11DeviceContext>& context);
	void UpdateWorldMatrix(DirectX::SimpleMath::Matrix worldRow);
	void UpdateBuffer(ComPtr<ID3D11DeviceContext>& context);
	DirectX::SimpleMath::Matrix GetWorldMatrix();
	
	void SetMaterials(const float roughness, const float metallic,
		const DirectX::SimpleMath::Vector3 albedo = DirectX::SimpleMath::Vector3(0.0f));
	// Attaching textures if model loader failed to find textures
	// should be called before Init()
	void AttachAlbedoTexture(std::string filename, int partNumber = 0);
	void AttachNormalTexture(std::string filename, int partNumber = 0);
	void AttachHeightTexture(std::string filename, int partNumber = 0);

protected:
	ComPtr<ID3D11Buffer> m_modelBufferGPU;

public:
	ModelBuffer m_modelBufferCPU;

	std::vector<Mesh> m_meshes;
};