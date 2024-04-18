#pragma once

#include <vector>
#include <unordered_map>

namespace Tofino
{
	using ObjectID = uint32_t;

	class IComponentContainer
	{
	public:
		virtual ~IComponentContainer() = default;
	};

	template<typename T>
	class ComponentContainer : public IComponentContainer
	{
	public:
		ComponentContainer() = default;
		~ComponentContainer() override = default;

		void Add(const ObjectID objID, T component)
		{
			assert(m_indexMap.find(objID) == m_indexMap.end());

			m_components.push_back(component);
			m_indexMap[objID] = m_components.size() - 1;
			m_idMap[m_components.size() - 1] = objID;
		}

		void Remove(const ObjectID objID)
		{
			assert(m_indexMap.find(objID) != m_indexMap.end());

			int last = m_components.size() - 1;
			int idx = m_indexMap[objID];

			m_components[idx] = m_components[last];
			m_components.pop_back();

			m_indexMap.erase(objID);
			m_idMap.erase(idx);
		}

		T& Get(const ObjectID objID)
		{
			assert(m_indexMap.find(objID) != m_indexMap.end());

			return m_components[m_indexMap[objID]];
		}

		bool Has(const ObjectID objID)
		{
			return m_indexMap.find(objID) != m_indexMap.end();
		}

		auto begin() { return m_components.begin(); }
		auto cbegin() const { return m_components.begin(); }
		auto end() { return m_components.end(); }
		auto cend() const { return m_components.cend(); }

	private:
		std::vector<T> m_components;
		std::unordered_map<ObjectID, int> m_indexMap; // id to index
		std::unordered_map<int, ObjectID> m_idMap; // index to id

	};
}