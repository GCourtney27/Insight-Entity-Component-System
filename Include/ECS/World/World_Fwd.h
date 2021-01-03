#pragma once

#include "ECS/Component/Component_Util.h"
#include "ECS/DataStructures/Component_Map.h"

#include <functional>



#if defined (_MSC_VER)
	#if defined (_DEBUG)
		#define DEBUG
	#endif // _DEBUG
#endif // _MSC_VER


#define ECS_NO_DISCARD [[nodiscard]]
#define ECS_FORCE_INLINE __forceinline



namespace ECS {

	/*
		Data Layout diagram
		-------------------
	
											 m_WorldComponents [Map]
														|
														v
		(Key) ComponentHash [ComponentHash_t]			-			(Value) m_Components [Map]
																				  |
																				  v
															(Key) Actor [Actor_t] - (Value) RawComponentData [Vector<UserComponentType>]
	*/

	namespace WorldHelpers
	{
		/*
			Returns a pointer to the component map in the world.

			@param (TemplateParam) ComponentMapType - The componenent type the map to retrieve holds.
			@param pBaseMap - A pointer to a 'World::m_WorldComponents' value.
		*/
		template <typename ComponentMapType>
		ECS_FORCE_INLINE constexpr auto GetComponentMap(ComponentMapBase* pBaseMap)
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

		std::size_t m_AvailableActorIndex;

	public:
		World()
			: m_AvailableActorIndex(0)
		{}
		~World()
		{
			printf("Cleaning Up World (%i bytes).\n", static_cast<unsigned int>(sizeof(*this)));

			Flush();
		}

		/*
			Destroy the world and all component maps and actors.
		*/
		void Flush()
		{
			for (auto& [Id, Map] : m_WorldComponents)
				delete Map;

			m_AvailableActorIndex = 0;
			m_WorldComponents.clear();
		}



		/*
			Adds a component to the world and returns a pointer to the new instance. 
			Returns nullptr if creation fails.
			@param Actor - The actor to attach the component too.
			@param Args - Arguments for the componnets constructor.
		*/
		template <typename ComponentType, typename ... InitArgs>
		ComponentType* AddComponent(Actor_t Actor, InitArgs ... Args)
		{
			// Verify input values.
			ValidateComponent<ComponentType>();
			if (!IsValidActor(Actor)) // Trying to get a component for an invalid actor.
				return static_cast<ComponentType*>(nullptr);


			using SpecializedMap = GenericComponentMap<ComponentType>;
			// Fetch the hashed id of the component type.
			ComponentHash_t ComponentHash = ECS::ComponentHash<ComponentType>();
			// Locate the hashed component id in the world.
			auto Iter = m_WorldComponents.find(ComponentHash);
			if (Iter != m_WorldComponents.end())
			{
				auto ComponentMap = WorldHelpers::GetComponentMap<ComponentType>(Iter->second);
				return ComponentMap->AddComponent(Actor, Args...);
			}
			else
			{
				m_WorldComponents[ComponentHash] = new SpecializedMap();
				auto ComponentMap = WorldHelpers::GetComponentMap<ComponentType>(m_WorldComponents[ComponentHash]);
				return ComponentMap->AddComponent(Actor, Args...);
			}

			return static_cast<ComponentType*>(nullptr);
		}

		/*
			Gets a pointer to the component from the world with its uniue ID.
			Returns nullptr if no component exists for it.
			@param ComponentId - The unique id of the component to get.
		*/
		template <typename ComponentType>
		ECS_NO_DISCARD ComponentType* GetComponentById(const ComponentUID_t& ComponentId)
		{
			// Verify input values.
			ValidateComponent<ComponentType>();
			
			using SpecializedMap = GenericComponentMap<ComponentType>;
			// Fetch the hashed id of the component type.
			ComponentHash_t ComponentHash = ECS::ComponentHash<ComponentType>();

			// Locate the component type in the world.
			auto Iter = m_WorldComponents.find(ComponentHash);
			if (Iter != m_WorldComponents.end())
			{
				// Find the actor that owns the component of that type.
				auto ComponentMap = WorldHelpers::GetComponentMap<ComponentType>(Iter->second);
				return ComponentMap->GetComponentById(ComponentId);
			}

			return static_cast<ComponentType*>(nullptr);
		}

		/*
			Remove a component from an actor by its unique ID.
			@param ComponentId - The unique id of the component to delete.
		*/
		template <typename ComponentType>
		void RemoveComponentById(const ComponentUID_t& ComponentId)
		{
			// Verify input values.
			ValidateComponent<ComponentType>();
			
			// Fetch the hashed id of the component type.
			ComponentHash_t ComponentHash = ECS::ComponentHash<ComponentType>();

			// Find the component in the world.
			auto Iter = m_WorldComponents.find(ComponentHash);
			if (Iter != m_WorldComponents.end())
			{
				// Ge the map and find the actor holding the instances of the components.
				auto ComponentMap = WorldHelpers::GetComponentMap<ComponentType>(Iter->second);
				ComponentMap->RemoveComponentById(ComponentId);
			}
		}

		/*
			Destroy an actor and its associated components from the world.
			@param Actor - The actor to destroy.
		*/
		void DestroyActor(Actor_t& Actor)
		{
			if (!IsValidActor(Actor)) // Trying to get a component for an invalid actor.
				return;

			// Remove all the componets associated with the actor.
			for (auto& [Id, Map] : m_WorldComponents)
				Map->DestroyActorRefs(Actor);

			InvalidateActor(Actor);
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
