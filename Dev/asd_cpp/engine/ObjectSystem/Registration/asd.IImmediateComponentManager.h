#pragma once

namespace asd
{
	template<typename TComponent>
	class IImmediateComponentManager
	{
	protected:
		typedef std::shared_ptr<TComponent> ComponentPtr;

	public:
		virtual void ImmediatelyAddComponent(const ComponentPtr& component, astring key) = 0;
		virtual void ImmediatelyRemoveComponent(astring key) = 0;
	};
}