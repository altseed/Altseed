#pragma once

namespace ace
{
	class ObjectComponent
	{
		friend class Object2DBase;

	private:
		void Update();

	protected:
		virtual void OnUpdate() = 0;

	public:
		virtual ~ObjectComponent()
		{
		}
	};
}