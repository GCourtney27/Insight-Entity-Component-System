#pragma once
#include <cassert>

namespace ECS
{

	class SystemBase
	{
	protected:
		const char* m_DebugName;
	protected:
		SystemBase(const char* DebugName)
			: m_DebugName(DebugName)
		{
		}
		virtual ~SystemBase() = default;
	};


	template <typename ComponentType>
	class GenericSystem : public SystemBase
	{
	protected:
		GenericComponentMap<ComponentType>* m_pComponentMapRef;

	public:
		GenericSystem(World& World, const char* DebugName = "")
		: SystemBase(DebugName)
		{
			m_pComponentMapRef = World.GetComponentMap<ComponentType>();
			assert(m_pComponentMapRef != nullptr); // Cannot make a system with a null component map to execute on.
		}
		virtual ~GenericSystem() {}

		/*
			Executes the system for all components.
		*/
		virtual void Execute() = 0;

		const char* GetDebugName() const
		{
			return m_DebugName;
		}

		/*
			Returns a reference to the container holding the raw compoenent data.
		*/
		inline std::vector<ComponentType>& GetRawComponentData()
		{
			return (*m_pComponentMapRef).m_RawComponents;
		}
	};

}
