#pragma once

namespace asd
{
	template<typename TComponent>
	class IComponentRegisterable
	{
	public:
		virtual ~IComponentRegisterable()
		{
		}
		virtual void Register(const std::shared_ptr<TComponent>& component) = 0;
		virtual void Unregister(const std::shared_ptr<TComponent>& component) = 0;
	};
}