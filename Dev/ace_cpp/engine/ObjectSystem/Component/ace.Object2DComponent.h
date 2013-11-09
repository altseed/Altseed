#pragma once

namespace ace
{
	class Object2D;

	class Object2DComponent
	{
		friend class Object2D;

	private:
		Object2D* m_object;

		void Update();
		void SetOwner(Object2D* value);

	protected:
		virtual void OnUpdate() = 0;

	public:
		Object2DComponent();
		virtual ~Object2DComponent()
		{
		}

		Object2D* GetOwner() const;
	};
}
