#pragma once
#include <stdint.h>

namespace ECS 
{

	/*
		An actor in the world. Typically called an entity.
	*/
	typedef uint32_t Actor_t;

	/*
		A invalid actor instance.
	*/
	static const Actor_t InvalidActor = -1;
	
	/*
		Returns true if the actor is valid, false if not.
	*/
	inline bool IsValidActor(const Actor_t& Actor)
	{
		return Actor != InvalidActor;
	}

	/*
		Invalidates an actor. WARNING: does not release component references.
	*/
	inline void InvalidateActor(Actor_t& Actor)
	{
		Actor = InvalidActor;
	}
}
