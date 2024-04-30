#pragma once

#include <string>

#include "Scene.h"
#include "ConstantBuffer.h"

using Microsoft::WRL::ComPtr;

namespace Tofino
{
	using ObjectID = uint32_t;

	class Mesh;

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

	class Hittable
	{
	public:
		bool hitEnabled = false;
		DirectX::BoundingSphere m_boundingSphere;
	};

	class Object : public Hittable
	{
	public:
		Object(Scene* scene, const std::string name = "Untitled", const bool isHittable = true);
		virtual ~Object() = default;

		virtual void Init(ComPtr<ID3D11Device>& device);
		void Bind(ComPtr<ID3D11DeviceContext>& context) const { m_constBuffer.Bind(context); }
		void RenderGUI();

		Matrix GetWorldMatrix()				{ return m_constBuffer.GetData().world.Transpose(); }
		void UpdateWorldMatrix(const Matrix worldRow);
		void UpdateWorldMatrix();

		virtual void Update(float deltaTime);

		ConstantBuffer<ModelBuffer>& GetConstBuffer();
		void UpdateConstBuffer(ComPtr<ID3D11DeviceContext>& context);

		void SetMeshMaterialFactors(
			const Vector4 baseColor, const float roughness, const float metallic, const int partNumber);
		void SetAllMeshMaterialFactors(
			const Vector4 baseColor, const float roughness, const float metallic);

		void SetUpdateFlag(bool flag)				{ m_updateFlag = flag;	}
		bool IsUpdateFlagSet() const				{ return m_updateFlag;	}
		std::string GetName() const					{ return m_name;		}
		ObjectID GetID() const						{ return m_id;			}

		template<typename T>
		void AddComponent(const T componentData)	{ m_scene->AddComponentOf(m_id, componentData); }
		template<typename T>
		T& GetComponent()							{ return m_scene->GetComponentOf<T>(m_id);		}
		template<typename T>
		bool HasComponent()							{ return m_scene->HasComponentOf<T>(m_id);		}

	protected:
		std::string m_name;
		bool m_updateFlag = false;

		ObjectID m_id;
		Scene* m_scene;

		ConstantBuffer<ModelBuffer> m_constBuffer;
	};

}
