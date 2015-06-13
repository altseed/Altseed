#include "asd.Layer2DComponent.h"
#include "../2D/asd.Layer2D.h"
#include "../asd.Layer.h"

namespace asd
{
	Layer2DComponent::Layer2DComponent()
		: m_layer(nullptr)
		, m_isUpdated(true)
		, m_isAlive(true)
	{
	}

	void Layer2DComponent::Update()
	{
		if (m_isUpdated)
		{
			OnUpdate();
		}
	}

	Layer2D* Layer2DComponent::GetOwner() const
	{
		return m_layer;
	}

	void Layer2DComponent::SetOwner(Layer2D* value)
	{
		m_layer = value;
	}

	bool Layer2DComponent::GetIsUpdated() const
	{
		return m_isUpdated;
	}

	void Layer2DComponent::SetIsUpdated(bool value)
	{
		m_isUpdated = value;
	}

	bool Layer2DComponent::GetIsAlive() const
	{
		return m_isAlive;
	}

	void Layer2DComponent::Vanish()
	{
		m_isAlive = false;
	}
}