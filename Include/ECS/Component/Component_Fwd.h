#pragma once

#include <string_view>


namespace ECS 
{

	/*
		Hashed identifier for component type. Unique
		for each component, but not each instance.
	*/
	typedef uint64_t ComponentHash_t;

	/*
		Unique identifier for each component instance.
	*/
	typedef uint64_t ComponentUID_t;

	/*
		Base class for all components that wish to be used by the code ECS system.
		Uses the curiously recurring template (CRT) pattern https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
		where the derived class is the template parameter for the base class.
	*/
	template <typename ComponentType>
	struct ComponentBase
	{
	protected:
		ComponentUID_t m_UID;
		Actor_t m_Owner;
	public:

		/*
			returns the components unique ID.
		*/
		inline ComponentUID_t GetId() const noexcept(true) { return m_UID; }

		/*
			Set the components unique ID.
		*/
		inline void SetID(const ComponentUID_t& ID) noexcept(true) { m_UID = ID; }

		bool operator ==(const ComponentType& rhs)
		{
			return this->GetId() == rhs.GetID();
		}


	protected:
		ComponentBase()
		{
			// Zero the component.
			::memset(this, 0, sizeof(ComponentBase<ComponentType>));

			// Register the ID.
			m_UID = s_ComponentID++;
		}
		virtual ~ComponentBase() = default;

	private:
		static ComponentUID_t s_ComponentID;
	};

	template <typename ComponentType>
	ComponentUID_t ComponentBase<ComponentType>::s_ComponentID = 0;

}