#pragma once

#include "ECS/Component/Component_Fwd.h"

#include <string_view>


namespace ECS 
{
	/*
		Compile-time check to ensure a component is of a type the code ECS can understand.
	*/
	template <class TargetComponent>
	constexpr void ValidateComponent()
	{
		constexpr bool IsValid = std::is_base_of<ComponentBase<TargetComponent>, TargetComponent>::value;
		static_assert(IsValid, "Target component is not a valid component.");
	}

	/*
		Hashes a component into a faster lookup value.
	*/
	template <typename ComponentType>
	inline ComponentHash_t ComponentHash()
	{
		// Make sure the passed in object is a valid component.
		ValidateComponent<ComponentType>();
		
		// Hash the component name and return it.
		std::string_view Name = typeid(ComponentType).name();
		return (ComponentHash_t)std::hash<std::string_view>()(Name);
	}
}