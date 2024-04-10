#pragma once

#include <string>
#include <vector>

#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "ConstantBuffer.h"

namespace Tofino
{
	// 16Byte align
	struct ModelBuffer // Must Store as a Column Matrix 
	{
		Matrix world;
		Matrix worldIT;

		// for test 
		int activeAlbedoMap = 1;
		int activeNormalMap = 1;
		int activeHeightMap = 1;
		int padding;
	};

	using Microsoft::WRL::ComPtr;

	//struct PickingVolume
	//{
	//	DirectX::BoundingSphere Sphere;
	//	float Dx, Dy, Dz;
	//};

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

		void Init(ComPtr<ID3D11Device>& device);

		void Bind(ComPtr<ID3D11DeviceContext>& context) const;

		void UpdateWorldMatrix(Matrix worldRow);

		//Transform& GetTransform(); 
		void SetLocation(Vector3 location);

		// Sets XYZ rotation order
		void SetRotation(Vector3 rotation);

		void SetScale(Vector3 scale);

		virtual void Update(float deltaTime);

		ConstantBuffer<ModelBuffer>& GetConstBuffer();

		void UpdateBuffer(ComPtr<ID3D11DeviceContext>& context);

		Matrix GetWorldMatrix();

		void SetMeshMaterialFactors(
			const Vector4 baseColor, const float roughness, const float metallic, const int partNumber);
		void SetAllMeshMaterialFactors(
			const Vector4 baseColor, const float roughness, const float metallic);

		void SetUpdateFlag(bool flag);

		bool IsUpdateFlagSet() const;

		std::string GetName() const;

		std::vector<Mesh>& GetMeshes();

	public:
		Transform m_transform;

	protected:
		std::string m_name;
		bool m_updateFlag = false;

		std::vector<Mesh> m_meshes;
		ConstantBuffer<ModelBuffer> m_constBuffer;
	};

}