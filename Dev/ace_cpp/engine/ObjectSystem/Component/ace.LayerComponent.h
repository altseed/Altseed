#pragma once

namespace ace
{
	class LayerComponent
	{
		friend class Layer2D;

	private:
		void Update();

	protected:
		virtual void OnUpdate() = 0;

	public:
		virtual ~LayerComponent()
		{
		}
	};
}