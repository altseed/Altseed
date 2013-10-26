#pragma once

namespace ace
{
	class Component
	{
		friend class Object2DBase;

	private:
		void Update();

	protected:
		virtual void OnUpdate() = 0;

	public:
		virtual ~Component()
		{
		}
	};
}