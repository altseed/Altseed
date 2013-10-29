#include "ace.LayerComponent.h"

namespace ace
{
	LayerComponent::LayerComponent()
		: m_layer(nullptr)
	{
	}

	void LayerComponent::Update()
	{
		OnUpdate();
	}

	Layer* LayerComponent::GetLayer() const
	{
		return m_layer;
	}

	void LayerComponent::SetLayer(Layer* value)
	{
		m_layer = value;
	}
}