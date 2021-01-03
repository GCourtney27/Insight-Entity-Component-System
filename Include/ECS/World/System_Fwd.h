#pragma once


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

		virtual void Execute() = 0;

	};


	template <typename ComponentType>
	class GenericSystem : public SystemBase
	{
	protected:
		GenericComponentMap<ComponentType>* m_pComponentMapRef;

	public:
		GenericSystem(GenericComponentMap<ComponentType>* pExecutee, const char* DebugName)
		: m_pComponentMapRef(pExecutee), SystemBase(DebugName)
		{
			assert(m_pComponentMapRef != nullptr); // Cannot make a system with a null component map to execute on.
		}
		virtual ~GenericSystem() {}

		virtual void Execute() override
		{
			size_t NumComponents = m_pComponentMapRef->GetNumComponents();
			for (size_t i = 0; i < NumComponents; ++i)
			{

			}
		}
		
	};

}
