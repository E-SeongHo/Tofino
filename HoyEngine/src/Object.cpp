
#include "Object.h"
#include "HashStringManager.h"

#include "SimpleMath.h"
#include "Mesh.h"

#include <iostream>

namespace Tofino
{
	Object::Object(Scene* scene, const std::string name, const bool isHittable)
		: m_constBuffer(ConstantBuffer<ModelBuffer>(VERTEX_SHADER | PIXEL_SHADER, 0))
	{
		hitEnabled = isHittable;
		m_name = name;

		int index = 0;
		while (!HashStringManager::IsAvailableName(m_name))
		{
			m_name = name + std::to_string(index++);
			std::cout << m_name << std::endl;
		}

		m_id = HashStringManager::GenerateHash(m_name);
		m_scene = scene;
	}

	void Object::Init(ComPtr<ID3D11Device>& device)
	{
		using namespace DirectX;

		assert(HasComponent<TransformComponent>());

		m_constBuffer.Init(device);

		// Assigns bounding sphere
		if(HasComponent<MeshComponent>())
		{
			Vector3 vmin(1000, 1000, 1000);
			Vector3 vmax(-1000, -1000, -1000);

			auto& meshes = GetComponent<MeshComponent>().Meshes;

			for (auto& mesh : meshes)
			{
				for (auto& v : mesh.GetVertexBuffer().GetData())
				{
					vmin.x = XMMin(vmin.x, v.pos.x);
					vmin.y = XMMin(vmin.y, v.pos.y);
					vmin.z = XMMin(vmin.z, v.pos.z);
					vmax.x = XMMax(vmax.x, v.pos.x);
					vmax.y = XMMax(vmax.y, v.pos.y);
					vmax.z = XMMax(vmax.z, v.pos.z);
				}
			}

			const float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
			const float r = XMMin(XMMin(dx, dy), dz) / 2.0f;

			const Vector3 center = GetComponent<TransformComponent>().Translation;
			m_boundingSphere = BoundingSphere(center, r);
		}

	}

	Matrix Object::GetWorldMatrix()
	{
		return m_constBuffer.GetData().world.Transpose();
	}

	void Object::UpdateWorldMatrix(const Matrix worldRow)
	{
		Matrix worldColumn = worldRow.Transpose();

		m_constBuffer.GetData().world = worldColumn;
		m_constBuffer.GetData().worldIT = worldColumn;
		m_constBuffer.GetData().worldIT.Translation(Vector3(0.0f));
		m_constBuffer.GetData().worldIT.Invert().Transpose();

		// hack : will be completed after build BVH system
		m_boundingSphere.Center = worldRow.Translation();

		Vector3 scale = { worldRow._11, worldRow._22, worldRow._33 };

		static float prevScale = 1.0f;
		float currentMaxScale = std::max(std::max(scale.x, scale.y), scale.z);
		float scaleFactor = currentMaxScale / prevScale;
		prevScale = currentMaxScale;

		m_boundingSphere.Radius = m_boundingSphere.Radius * scaleFactor;

		m_updateFlag = true;
	}

	void Object::UpdateWorldMatrix()
	{
		assert(HasComponent<TransformComponent>());

	}

	void Object::Update(float deltaTime)
	{
	}

	ConstantBuffer<ModelBuffer>& Object::GetConstBuffer()
	{
		return m_constBuffer;
	}

	void Object::UpdateConstBuffer(ComPtr<ID3D11DeviceContext>& context)
	{
		m_constBuffer.Update(context);
		m_updateFlag = false;
	}

	void Object::SetMeshMaterialFactors(Vector4 baseColor, const float roughness, const float metallic, const int partNumber)
	{
		std::vector<Mesh>& meshes = GetComponent<MeshComponent>().Meshes;
		if (partNumber >= 0 && partNumber < meshes.size())
		{
			meshes[partNumber].SetMaterialFactors(baseColor, roughness, metallic);
		}
	}

	void Object::SetAllMeshMaterialFactors(const Vector4 baseColor, const float roughness, const float metallic)
	{
		std::vector<Mesh>& meshes = GetComponent<MeshComponent>().Meshes;
		for (auto& mesh : meshes)
		{
			mesh.SetMaterialFactors(baseColor, roughness, metallic);
		}
	}
}