#pragma once

#include <unordered_map>
#include <any>

#include "ECS/World/Actor_Fwd.h"

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

		virtual void DestroyActor(const Actor_t& Actor) = 0;

	};

	/*
		* Container for a list of components.
		* An actor can have multiple instances of the same type of component, so 
		  the raw component data is stored in a resizable vector.
	*/
	template <typename ComponentType>
	class GenericComponentMap : public ComponentMapBase
	{
	protected:
		std::unordered_map<Actor_t, std::vector<ComponentType>> m_Components;

	public:
		GenericComponentMap()	= default;

		virtual ~GenericComponentMap()
		{
			printf("[WARNING] Component map being destroyed.\n");
		}

		/*
			Destroys an actor in the world.
			Destroying one actor in the map will destroy all instances of this map's
			component storage type.
		*/
		virtual void DestroyActor(const Actor_t& Actor) override
		{
			auto ActorLocationIter = m_Components.find(Actor);
			m_Components.erase(ActorLocationIter);
		}

		/*
			Returns a reference to the underlying data structor for this component map.
		*/
		std::unordered_map<Actor_t, std::vector<ComponentType>>& GetUnorderedMap() { return m_Components; }

		/*
			Returns a iterator to the beginning of the underlying component map.
		*/
		typename std::unordered_map<Actor_t, std::vector<ComponentType>>::iterator begin()	{ return m_Components.begin(); }
		
		/*
			Returns a iterator to the end of the underlying component map.
		*/
		typename std::unordered_map<Actor_t, std::vector<ComponentType>>::iterator end()		{ return m_Components.end(); }

		/*
			Find a value in the underlying map given the actor key.
		*/
		typename std::unordered_map<Actor_t, std::vector<ComponentType>>::iterator find(Actor_t Key)	 	{ return m_Components.find(Key); }
		
		/*
			Erases a element from the underlying map givven a iterator location.
		*/
		void erase(typename std::unordered_map<Actor_t, std::vector<ComponentType>>::iterator Location)	{ m_Components.erase(Location); }


	};

}
