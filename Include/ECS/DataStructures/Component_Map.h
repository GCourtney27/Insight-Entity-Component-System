#pragma once

#include "ECS/World/Actor_Fwd.h"
#include "ECS/Component/Component_Fwd.h"

#include <unordered_map>


namespace ECS
{
	/*
		* The base for a generic component map that an hold any data structure.
		* NOT to be directly instantiated.
	*/
	class ComponentMapBase
	{
	public:
		ComponentMapBase() = default;
		virtual ~ComponentMapBase() = default;

		virtual void DestroyActorRefs(const Actor_t& Actor) = 0;

	};

	/*
		A container for  list of componenet types.
		* Components are garenteed to be linear in memory for a higher cache hit rate upon iteration.
	*/
	template <typename ComponentType>
	class GenericComponentMap : public ComponentMapBase
	{
		friend class World;
	private:
		typedef uint32_t	ArrayIndex;

	public:
		std::unordered_map<ComponentUID_t, std::pair<Actor_t, ArrayIndex>> m_ComponentMap;
		std::vector<ComponentType> m_RawComponents;
		uint32_t m_NextAvailableIndex;

		GenericComponentMap()
			: m_NextAvailableIndex(0u)
		{
			// Verify th template argument is a valid component.
			ValidateComponent<ComponentType>();
		}

		virtual ~GenericComponentMap()
		{
			size_t ComponentsSize = m_RawComponents.size() * sizeof(ComponentType);
			size_t ComponentMapSize = m_ComponentMap.size() * (sizeof(ComponentUID_t) + sizeof(std::pair<Actor_t, ArrayIndex>));
			printf("[WARNING] Generic ComponentMap being destroyed. Raw component memory [%zi] bytes | Component map [%zi] bytes\n", ComponentsSize, ComponentMapSize);
		}

		/*
			Returns the size of the underlying component container in bytes.
		*/
		size_t GetContainerSize() const
		{
			return m_RawComponents.size() * sizeof(ComponentType);
		}

		template <typename ... Args>
		ComponentType* AddComponent(const Actor_t& Owner, Args ... args)
		{
			m_RawComponents.push_back(ComponentType(args...));
			ComponentUID_t id = m_RawComponents[m_NextAvailableIndex].GetID();
			m_ComponentMap[id] = std::make_pair(Owner, m_NextAvailableIndex);
			m_NextAvailableIndex++;

			return &m_RawComponents[m_NextAvailableIndex - 1];
		}

		void RemoveComponentById(ComponentUID_t Id)
		{
			ArrayIndex i = m_ComponentMap[Id].second;
			std::vector<ComponentType>::iterator Iter = m_RawComponents.begin();
			Iter += i;
			m_RawComponents.erase(Iter);
			m_ComponentMap.erase(Id);
			m_NextAvailableIndex--;
		}

		ComponentType* GetComponentById(ComponentUID_t Id)
		{
			auto Iter = m_ComponentMap.find(Id);
			if (Iter != m_ComponentMap.end())
				return &m_RawComponents[Iter->second.second];
			else
				return nullptr;
		}

		ComponentType& operator[](uint32_t Index)
		{
			return m_RawComponents[Index];
		}

		virtual void DestroyActorRefs(const Actor_t& Actor) override
		{
			std::vector<ComponentType>::iterator Iter;
			std::unordered_map<ComponentUID_t, std::pair<Actor_t, ArrayIndex>>::iterator MapIter = m_ComponentMap.begin();
			int i = 0;
			for (auto& [CompUID, Pair] : m_ComponentMap)
			{
				if (Pair.first == Actor)
				{
					Iter = m_RawComponents.begin() + Pair.second;
					m_RawComponents.erase(Iter);
					m_ComponentMap.erase(MapIter);
				}
				if (m_ComponentMap.size() > 0)	// We could have deleted the only component in this map if there was only one.
					MapIter++;
				else
					break;
			}
		}
	};
	
}
