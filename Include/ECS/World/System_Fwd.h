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
	public:
		GenericSystem() {}
		virtual ~GenericSystem() {}
	
		virtual void Execute() override
		{

		}
		
	};

}
