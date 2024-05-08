#pragma once

#include <string>

#include "Scene.h"
#include "ConstantBuffer.h"

using Microsoft::WRL::ComPtr;

namespace Tofino
{
	using ObjectID = uint32_t;

	class InstanceGroup;
	struct Collision;

	struct ObjectStatusBuffer
	{
		int isInstanced = 0;
		int activeAlbedoMap = 1;
		int activeNormalMap = 1;
		int activeHeightMap = 1;
	};

	struct ModelBuffer // Must Store as a Column Matrix 
	{
		Matrix world;
		Matrix worldIT;
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
		void Bind(ComPtr<ID3D11DeviceContext>& context) const { m_modelConstBuffer.Bind(context); m_statusConstBuffer.Bind(context); }
		void RenderGUI();

		Matrix GetWorldMatrix()				{ return m_modelConstBuffer.GetData().world.Transpose(); }
		void UpdateWorldMatrix(const Matrix worldRow);
		void UpdateWorldMatrix();

		virtual void Update(float deltaTime);
		virtual void OnCollisionDetected(Collision& collision) {}

		ConstantBuffer<ModelBuffer>& GetConstBuffer();
		void UpdateConstBuffer(ComPtr<ID3D11DeviceContext>& context);

		// TODO : MeshComponent
		void SetMeshMaterialFactors(
			const Vector4 baseColor, const float roughness, const float metallic, const int partNumber);
		void SetAllMeshMaterialFactors(
			const Vector4 baseColor, const float roughness, const float metallic);

		void SetUpdateFlag(bool flag)				{ m_updateFlag = flag;	}
		bool IsUpdateFlagSet() const				{ return m_updateFlag;	}
		std::string GetName() const					{ return m_name;		}
		ObjectID GetID() const						{ return m_id;			}

		template<typename T>
		void AddComponent()		{ m_scene->AddComponentOf<T>(m_id);				}

		template<typename T>
		T& GetComponent()		{ return m_scene->GetComponentOf<T>(m_id);		}

		template<typename T>
		bool HasComponent()		{ return m_scene->HasComponentOf<T>(m_id);		}

	protected:
		friend class InstanceGroup;
		friend class Scene; 

		ObjectID m_id;
		Scene* m_scene;
		std::string m_name;

		ConstantBuffer<ModelBuffer> m_modelConstBuffer;
		ConstantBuffer<ObjectStatusBuffer> m_statusConstBuffer;
		bool m_updateFlag = false;

		bool m_isInstancing = false;
		InstanceGroup* m_owningGroup = nullptr;
	};

}
