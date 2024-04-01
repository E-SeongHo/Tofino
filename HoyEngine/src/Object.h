#pragma once

#include <string>
#include <vector>

#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "ConstantBuffer.h"

// 16Byte align
struct ModelBuffer // Must Store as a Column Matrix 
{
	DirectX::SimpleMath::Matrix world;
	DirectX::SimpleMath::Matrix worldIT;

	// for test 
	int activeAlbedoMap = 1;
	int activeNormalMap = 1;
	int activeHeightMap = 1;
	int padding;
};

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
	
	void SetMeshMaterialFactors(
		const DirectX::SimpleMath::Vector4 baseColor, const float roughness, const float metallic, const int partNumber);
	void SetAllMeshMaterialFactors(
		const DirectX::SimpleMath::Vector4 baseColor, const float roughness, const float metallic);

	std::vector<Mesh>& GetMeshes();

public:
	Transform m_transform; 
	std::string m_name;

	std::vector<Mesh> m_meshes;
	
	bool m_updateFlag = false;

private:
	ConstantBuffer<ModelBuffer> m_constBuffer;

};