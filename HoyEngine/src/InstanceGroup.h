#pragma once

#include "Object.h"
#include "StructuredBuffer.h"

namespace Tofino
{
	// should i need to make this template class?
	class InstanceGroup
	{
	public:
		InstanceGroup(MeshComponent shareMeshComponent, std::string groupName, int numInstances);
		~InstanceGroup() = default;

		void Init(ComPtr<ID3D11Device>& device);
		void UpdateBuffer(ComPtr<ID3D11DeviceContext>& context);
		void Bind(ComPtr<ID3D11DeviceContext>& context) const
		{
			m_modelBuffers.Bind(context);
			m_statusBuffer.Bind(context);
		}

		UINT GetInstanceCount() const			{ return m_instances.size(); }
		std::vector<Object*>& GetInstances()	{ return m_instances; }

		MeshComponent& GetSharingMeshComponent() { return m_meshComponent; }

	private:
		friend class Scene;

		Scene* m_scene;
		std::string m_name;

		MeshComponent m_meshComponent;
		std::vector<Object*> m_instances;
		StructuredBuffer<ModelBuffer> m_modelBuffers;
		ConstantBuffer<ObjectStatusBuffer> m_statusBuffer;
	};

}
