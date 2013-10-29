#pragma once

namespace ace
{
	class LayerComponent
	{
		friend class Layer;

	private:
		Layer* m_layer;

		void Update();
		void SetLayer(Layer* value);

	protected:
		virtual void OnUpdate() = 0;

	public:
		LayerComponent();
		virtual ~LayerComponent()
		{
		}

		Layer* GetLayer() const;
	};
}