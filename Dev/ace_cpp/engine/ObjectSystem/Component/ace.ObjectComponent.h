#pragma once

namespace ace
{
	class ObjectComponent
	{
		friend class Object2DBase;

	private:
		Object2DBase* m_object;

		void Update();
		void SetOwner(Object2DBase* value);

	protected:
		virtual void OnUpdate() = 0;

	public:
		ObjectComponent();
		virtual ~ObjectComponent()
		{
		}

		Object2DBase* GetOwner() const;
	};
}