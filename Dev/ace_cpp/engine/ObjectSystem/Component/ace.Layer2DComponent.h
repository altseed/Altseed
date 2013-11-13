#pragma once

namespace ace
{
	class Layer2D;

	class Layer2DComponent
	{
		friend class Layer2D;

	private:
		Layer2D* m_layer;

		void Update();
		void SetOwner(Layer2D* value);

	protected:
		virtual void OnUpdate() = 0;

	public:
		Layer2DComponent();
		virtual ~Layer2DComponent()
		{
		}

		Layer2D* GetOwner() const;
	};
}
