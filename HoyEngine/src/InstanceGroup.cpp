
#include "InstanceGroup.h"

namespace Tofino
{
	InstanceGroup::InstanceGroup(MeshComponent sharedMeshComponent, std::string groupName, int numInstances)
		: m_meshComponent(sharedMeshComponent), m_name(groupName),
		  m_modelBuffers(VERTEX_SHADER, 20, numInstances),
          m_statusBuffer(VERTEX_SHADER | PIXEL_SHADER, 1)
	{
		m_instances.reserve(numInstances);
	}

	void InstanceGroup::Init(ComPtr<ID3D11Device>& device)
	{
		m_modelBuffers.Init(device);

		m_statusBuffer.GetData().isInstanced = true;
		m_statusBuffer.Init(device);

		assert(m_instances.size() == m_instances.capacity());

		for(auto instance : m_instances)
		{
			instance->m_isInstancing = true;
			instance->m_owningGroup = this;
		}
	}

	void InstanceGroup::UpdateBuffer(ComPtr<ID3D11DeviceContext>& context)
	{
		// doesn't really need to keep the same order between m_instances with structured buffer
		// it does! -> checking update flag, and update only if the flag set

		bool flag = false;
		for(int i = 0; i < m_instances.size(); i++)
		{
			auto instance = m_instances[i];
			assert(instance->m_isInstancing && instance->m_owningGroup == this);

			if (instance->m_updateFlag)
			{
				auto& src = instance->m_modelConstBuffer.GetData();

				m_modelBuffers.GetData()[i] = src;
				instance->m_updateFlag = false;
				flag = true;
			}
		}

		if (flag)
		{
			m_modelBuffers.Update(context);
			std::cout << "it's going to be a big update" << std::endl;
		}

		for(auto& mesh : m_meshComponent.Meshes)
		{
			if (mesh.IsUpdateFlagSet()) mesh.UpdateBuffer(RendererContext);
		}
	}
}
