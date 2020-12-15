#pragma once
#include <stdint.h>

namespace ECS {

	typedef uint64_t Actor_t;

	static const Actor_t InvalidActor = -1;
	
	inline bool IsValidActor(Actor_t Actor)
	{
		return Actor != InvalidActor;
	}
}
