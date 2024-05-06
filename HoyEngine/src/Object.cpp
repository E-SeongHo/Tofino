
#include <imgui.h>
#include <iostream>

#include "Object.h"
#include "HashStringManager.h"

#include "SimpleMath.h"
#include "Mesh.h"


namespace Tofino
{
	Object::Object(Scene* scene, const std::string name, const bool isHittable)
		: m_modelConstBuffer(ConstantBuffer<ModelBuffer>(VERTEX_SHADER, 0)),
		  m_statusConstBuffer(ConstantBuffer<ObjectStatusBuffer>(VERTEX_SHADER | PIXEL_SHADER, 1))
	{
		hitEnabled = isHittable;
		m_name = name;

		int index = 0;
		while (!HashStringManager::IsAvailableName(m_name))
		{
			m_name = name + std::to_string(index++);
			std::cout << name << " -> " << m_name << std::endl;
		}

		m_id = HashStringManager::GenerateHash(m_name);
		m_scene = scene;

		m_scene->RegisterObject(this);
	}

	void Object::Init(ComPtr<ID3D11Device>& device)
	{
		m_modelConstBuffer.Init(device);
		m_statusConstBuffer.Init(device);

		//if (!HasComponent<TransformComponent>()) AddComponent<TransformComponent>();

		auto& transform = GetComponent<TransformComponent>();
		const Vector3 center = transform.Translation;
		const float r = std::max(std::max(transform.Scale.x, transform.Scale.y), transform.Scale.z);
		m_boundingSphere = DirectX::BoundingSphere(center, r);

		m_updateFlag = true;
		/*UpdateWorldMatrix(Math::Transformer(transform));
		UpdateConstBuffer(RendererContext);*/
	}

	void Object::RenderGUI()
	{
		static ImGuiColorEditFlags flags = 
			ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | 
			ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_AlphaBar;

		if (ImGui::TreeNode(m_name.c_str()))
		{
			auto& transform = GetComponent<TransformComponent>();

			m_updateFlag += ImGui::SliderFloat3("Position", &transform.Translation.x, -100.0f, 100.0f);
			m_updateFlag += ImGui::SliderFloat3("Rotation", &transform.Rotation.x, -360.0f, 360.0f);
			m_updateFlag += ImGui::SliderFloat3("Scale", &transform.Scale.x, 0.0f, 10.0f);

			m_updateFlag += ImGui::CheckboxFlags("Albedo Map", &m_statusConstBuffer.GetData().activeAlbedoMap, 1);
			m_updateFlag += ImGui::CheckboxFlags("Height Map", &m_statusConstBuffer.GetData().activeHeightMap, 1);
			m_updateFlag += ImGui::CheckboxFlags("Normal Map", &m_statusConstBuffer.GetData().activeNormalMap, 1);

			if(HasComponent<CameraComponent>())
			{
				auto& camera = GetComponent<CameraComponent>().Camera;
				ImGui::SliderFloat3("Camera", &camera.GetOrigin().x, -100.0f, 100.0f);
			}

			if(HasComponent<MeshComponent>())
			{
				auto& meshes = GetComponent<MeshComponent>().Meshes;
				for(int i = 0; i < meshes.size(); i++)
				{
					int meshUpdateFlag = 0;
					if (ImGui::TreeNode((std::string("Part ") + std::to_string(i)).c_str()))
					{
						auto& materialData = meshes[i].GetMaterial().GetMaterialStatus();
						if (!m_statusConstBuffer.GetData().activeAlbedoMap)
						{
							meshUpdateFlag += ImGui::ColorPicker4("BaseColor", &materialData.baseColor.x, flags, NULL);
						}
						meshUpdateFlag += ImGui::SliderFloat("Roughness", &materialData.roughness, 0.0f, 1.0f);
						meshUpdateFlag += ImGui::SliderFloat("Metaillic", &materialData.metallic, 0.0f, 1.0f);

						ImGui::TreePop();
					}

					if (meshUpdateFlag) meshes[i].SetUpdateFlag(true);
				}
				ImGui::TreePop();
			}
		}
	}

	void Object::UpdateWorldMatrix(const Matrix worldRow)
	{
		Matrix worldColumn = worldRow.Transpose();

		m_modelConstBuffer.GetData().world = worldColumn;
		m_modelConstBuffer.GetData().worldIT = worldColumn;
		m_modelConstBuffer.GetData().worldIT.Translation(Vector3(0.0f));
		m_modelConstBuffer.GetData().worldIT.Invert().Transpose();

		// hack : will be completed after building BVH system
		m_boundingSphere.Center = worldRow.Translation();

		Vector3 scale = { worldRow._11, worldRow._22, worldRow._33 };

		float currentMaxScale = std::max(std::max(scale.x, scale.y), scale.z);

		m_boundingSphere.Radius = currentMaxScale;
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
		return m_modelConstBuffer;
	}

	void Object::UpdateConstBuffer(ComPtr<ID3D11DeviceContext>& context)
	{
		m_modelConstBuffer.Update(context);
		m_statusConstBuffer.Update(context);

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
