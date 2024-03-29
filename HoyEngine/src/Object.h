#pragma once

#include <string>
#include <vector>

#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "ConstantBuffer.h"

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
	Object(const std::string name = "Untitled", const bool isHittable = true);
	//virtual ~Object();

	// Load Textures, Create Buffers
	void Init(ComPtr<ID3D11Device>& device);
	
	void Render(ComPtr<ID3D11DeviceContext>& context);
	
	void RenderNormal(ComPtr<ID3D11DeviceContext>& context);

	void UpdateWorldMatrix(DirectX::SimpleMath::Matrix worldRow);
	
	//Transform& GetTransform(); 
	void SetLocation(DirectX::SimpleMath::Vector3 location);
	
	// Sets XYZ rotation order
	void SetRotation(DirectX::SimpleMath::Vector3 rotation); 
	
	void SetScale(DirectX::SimpleMath::Vector3 scale);

	virtual void Update(float deltaTime);

	// Gets const buffer of the object
	ConstantBuffer<ModelBuffer>& GetConstBuffer();

	void UpdateBuffer(ComPtr<ID3D11DeviceContext>& context);
	
	DirectX::SimpleMath::Matrix GetWorldMatrix();
	
	void SetMaterials(const float roughness, const float metallic,
		const DirectX::SimpleMath::Vector4 albedo = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	
	// Attaching textures manually if model loader failed to find textures
	// should be called before the Init Function called
	void AttachAlbedoTexture(std::string filename, int partNumber = 0);
	void AttachNormalTexture(std::string filename, int partNumber = 0);
	void AttachHeightTexture(std::string filename, int partNumber = 0);

public:
	Transform m_transform; 
	std::string m_name;

	std::vector<Mesh> m_meshes;
	
	bool m_updateFlag = false;

private:
	ConstantBuffer<ModelBuffer> m_constBuffer;

};