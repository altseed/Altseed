#pragma once

namespace ace
{
	class ObjectComponent
	{
		friend class Object2D;

	private:
		Object2D* m_object;

		void Update();
		void SetOwner(Object2D* value);

	protected:
		virtual void OnUpdate() = 0;

	public:
		ObjectComponent();
		virtual ~ObjectComponent()
		{
		}

		Object2D* GetOwner() const;
	};
}