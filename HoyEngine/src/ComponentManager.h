#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include "Components.h"
#include "ComponentContainer.h"


namespace Tofino
{
	class ComponentManager
	{
	public:
		ComponentManager()
		{
			InitContainers(AllComponents{});
		}
		~ComponentManager()
		{
			for(auto container : m_containers)
			{	
				delete container.second;
			}
		}

		template<typename T>
		void AddComponent(const ObjectID objID, const T& component)
		{
			GetContainer<T>().Add(objID, component);
		}

		template<typename T>
		void RemoveComponent(const ObjectID objID)
		{
			GetContainer<T>().Remove(objID);
		}

		template<typename T>
		T& GetComponent(const ObjectID objID)
		{
			return GetContainer<T>().Get(objID);
		}

		template<typename T>
		bool HasComponent(const ObjectID objID)
		{
			return GetContainer<T>().Has(objID);
		}

		template<typename T>
		ComponentContainer<T>& GetContainer()
		{
			const std::string componentName = std::string(typeid(T).name());
			assert(m_containers.find(componentName) != m_containers.end());

			ComponentContainer<T>* container =
				static_cast<ComponentContainer<T>*>(m_containers[componentName]);

			return *container;
		}

	private:
		template<typename T>
		void InitContainer()
		{
			const std::string componentName = std::string(typeid(T).name());

			m_containers[componentName] = new ComponentContainer<T>();

			std::cout << componentName << " init " << std::endl;
		}

		template<typename... Components>
		void InitContainers(ComponentGroup<Components...>)
		{
			(InitContainer<Components>(), ...);
		}

		std::unordered_map<std::string, IComponentContainer*> m_containers;

	};

}
