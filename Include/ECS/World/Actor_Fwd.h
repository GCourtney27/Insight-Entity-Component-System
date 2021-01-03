#pragma once
#include <stdint.h>

namespace ECS 
{

	typedef uint64_t Actor_t;
	static const Actor_t InvalidActor = -1;
	
	inline bool IsValidActor(const Actor_t& Actor)
	{
		return Actor != InvalidActor;
	}

	inline void InvalidateActor(Actor_t& Actor)
	{
		Actor = InvalidActor;
	}
}
