#pragma once

#include <functional>
#include <stdexcept>

#include "ECS/DataStructures/Component_Map.h"
#include "ECS/Component/Component_Util.h"



#define ECS_NO_DISCARD [[nodiscard]]

namespace ECS {

	/*
									 m_WorldComponents [Map]
												|
												v
		ComponentHash [ComponentHash_t]			-			m_Components [Map]
																  |
																  v
												  Actor [Actor_t] - RawComponentData [Vector<UserComponentType>]
	*/

	namespace WorldHelpers
	{
		/*
			Returns a pointer to the component map in the world.

			@param (TemplateParam) ComponentMapType - The componenent type the map to retrieve holds.
			@param pBaseMap - A pointer to a 'World::m_WorldComponents' value.
		*/
		template <typename ComponentMapType>
		inline auto GetComponentMap(ComponentMapBase* pBaseMap)
		{
			return dynamic_cast<GenericComponentMap<ComponentMapType>*>(pBaseMap);
		}
	}


	class World
	{
	protected:
		/*
			The container for all the components in the world.
		*/
		std::unordered_map<ComponentHash_t, ComponentMapBase*> m_WorldComponents;

		std::size_t m_AvailableActorIndex = 0;

	public:
		World() = default;
		~World()
		{
			for (auto& [Id, Map] : m_WorldComponents)
				delete Map;
		}

		/*
			Create a component and add it to the world. Returs a pointer to the new componenet, or 
			// nullptr if the component could not be created
			@param (TemplateParam) ComponentType - The type of component to create.
			@param Actor - The actor to attach the component too.
			@param (ParameterPack) Args - Default contructor arguments for the component to create.
		*/
		template <typename ComponentType, typename ... InitArgs>
		ComponentType* AddComponent(Actor_t Actor, InitArgs ... Args)
		{
			IsValidComponent<ComponentType>();
			using SpecializedMap = GenericComponentMap<ComponentType>;

			if (!IsValidActor(Actor)) throw std::logic_error("Trying to get a component for an invalid actor.");

			ComponentHash_t ComponentHash = ECS::ComponentHash<ComponentType>();

			// Locate the hashed component id in the world.
			auto Iter = m_WorldComponents.find(ComponentHash);
			if (Iter != m_WorldComponents.end())
			{
				auto ComponentMap = WorldHelpers::GetComponentMap<ComponentType>(Iter->second);
				auto ComponentMapActorLocationIter = ComponentMap->find(Actor);
				if (ComponentMapActorLocationIter != ComponentMap->end())
				{
					// Contruct the component in the actors list of component instances.
					ComponentMapActorLocationIter->second.emplace_back(Args...);

					// Return a pointer to the new component instance.
					return &ComponentMapActorLocationIter->second.back();
				}
			}
			// If the component does not exist in the world create a new component map
			// and add the component with it.
			else
			{
				// Create the new map on the heap.
				m_WorldComponents[ComponentHash] = new SpecializedMap();
				auto ComponentMap = WorldHelpers::GetComponentMap<ComponentType>(m_WorldComponents[ComponentHash]);

				// Create a new entry for the actor and its component instances.
				ComponentMap->GetUnorderedMap().try_emplace(Actor, std::vector<ComponentType>{});

				// Create the component without copying.
				ComponentMap->GetUnorderedMap()[Actor].emplace_back(Args...);

				// Return a pointer to the newly creaed component.
				return &ComponentMap->GetUnorderedMap()[Actor].back();
			}

			return static_cast<ComponentType*>(nullptr);
		}

		/*
			Get a component from the world with a specified actor.
			Returns nullptr if no component exists for it.
			@param (TemplateParam) ComponentType - The type of the component to delete.
			@param Owner - The actor to get the component from.
			@param ComponentId - The unique id of the component to get.
		*/
		template <typename ComponentType>
		ECS_NO_DISCARD ComponentType* GetComponentById(const Actor_t& Owner, const ComponentUID_t& ComponentId)
		{
			IsValidComponent<ComponentType>();
			using SpecializedMap = GenericComponentMap<ComponentType>;

			ComponentHash_t ComponentHash = ECS::ComponentHash<ComponentType>();

			// Locate the component type in the world.
			auto Iter = m_WorldComponents.find(ComponentHash);
			if (Iter != m_WorldComponents.end())
			{
				// Find the actor that owns the component of that type.
				auto ComponentMap = WorldHelpers::GetComponentMap<ComponentType>(Iter->second);
				auto ComponentMapActorLocationIter = ComponentMap->find(Owner);
				if (ComponentMapActorLocationIter != ComponentMap->end())
				{
					// Iterate over the components until we find the desired component.
					//
					// An actor could have multiple components of the same type so test their id
					// to get the proper instance.
					for (auto& RawComponent : ComponentMapActorLocationIter->second)
					{
						if (RawComponent.GetID() == ComponentId)
						{
							return &RawComponent;
						}
					}
				}
			}

			return static_cast<ComponentType*>(nullptr);
		}

		/*
			Remove a component from an actor by its unique ID.
			@param (TemplateParam) ComponentType - The type of the component to delete.
			@param Owner - The owner of the component to be deleted.
			@param ComponentId - The unique id of the component to delete.
		*/
		template <typename ComponentType>
		void RemoveComponentById(const Actor_t& Owner, const ComponentUID_t& ComponentId)
		{
			IsValidComponent<ComponentType>();

			ComponentHash_t ComponentHash = ECS::ComponentHash<ComponentType>();

			// Find the component in the world.
			auto Iter = m_WorldComponents.find(ComponentHash);
			if (Iter != m_WorldComponents.end())
			{
				// Ge the map and find the actor holding the instances of the components.
				auto ComponentMap = WorldHelpers::GetComponentMap<ComponentType>(Iter->second);
				auto MapIter = ComponentMap->find(Owner);
				if (MapIter != ComponentMap->end())
				{
					// Iteratate through the map and find the component that matches the specified ID.
					auto DeleteTargetIter = std::find_if(MapIter->second.begin(), MapIter->second.end(),
						[&ComponentId](const ComponentBase<ComponentType>& Comp)
						{
							return ComponentId == Comp.GetID();
						}
					);
					// Delete it.
					if (DeleteTargetIter != MapIter->second.end())
					{
						MapIter->second.erase(DeleteTargetIter);
					}
				}
				else
				{
					// No actor in the world has the component
				}
			}
		}

		/*
			Destroy an actor and its associated components from the world.
			@param Actor - The actor to destroy.
		*/
		void DestroyActor(const Actor_t& Actor)
		{
			// Remove all the componets associated with the actor.
			for (auto& [Id, Map] : m_WorldComponents)
			{
				Map->DestroyActor(Actor);
			}
		}

		/*
			Creates an actor and returns it. If creation fails, an invalid actor is returned.
		*/
		ECS_NO_DISCARD Actor_t CreateActor()
		{
			Actor_t Actor = static_cast<Actor_t>(m_AvailableActorIndex++);
			if (IsValidActor(Actor))
				return Actor;
			else
				return InvalidActor;
		}

	};
}
