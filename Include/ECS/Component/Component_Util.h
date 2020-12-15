#pragma once
#include <string_view>

namespace ECS {


	typedef uint64_t ComponentHash_t;
	typedef uint64_t ComponentUID_t;

	template <typename ComponentType>
	struct ComponentBase
	{
	protected:
		ComponentUID_t m_UID;

	public:
		inline ComponentUID_t GetID() const noexcept(true) { return m_UID; }
		inline void SetID(const ComponentUID_t& ID) noexcept(true) { m_UID = ID; }

		bool operator ==(const ComponentType& rhs)
		{
			return this->GetID() == rhs.GetID();
		}
		

	protected:
		ComponentBase()
			: m_UID(s_ComponentID++)
		{
			::memset(this, 0, sizeof(ComponentBase<ComponentType>));
		}
		virtual ~ComponentBase() = default;

	private:
		static ComponentUID_t s_ComponentID;
	};
	
	template <typename ComponentType>
	ComponentUID_t ComponentBase<ComponentType>::s_ComponentID = 0;



	template <typename ComponentType>
	inline ComponentHash_t ComponentHash()
	{
		// Make sure the passed in object is a valid component.
		constexpr bool IsValid = std::is_base_of<ComponentBase<ComponentType>, ComponentType>::value;
		static_assert(IsValid, "Object is not a valid component.");

		// Hash the component name and return it.
		std::string_view Name = typeid(ComponentType).name();
		return (ComponentHash_t)std::hash<std::string_view>()(Name);
	}

	template <class TargetComponent>
	constexpr void IsValidComponent()
	{
		constexpr bool IsValid = std::is_base_of<ComponentBase<TargetComponent>, TargetComponent>::value;
		static_assert(IsValid, "Target component is not a valid component.");
	}

}