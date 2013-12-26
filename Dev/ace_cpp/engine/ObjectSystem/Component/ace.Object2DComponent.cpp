#include "ace.Object2DComponent.h"

namespace ace
{
	Object2DComponent::Object2DComponent()
		: m_owner(nullptr)
		, m_isUpdated(true)
		, m_isAlive(true)
	{
	}

	void Object2DComponent::Update()
	{
		if (m_isUpdated)
		{
			OnUpdate();
		}
	}

	Object2D* Object2DComponent::GetOwner() const
	{
		return m_owner;
	}

	void Object2DComponent::SetOwner(Object2D* value)
	{
		m_owner = value;
	}

	bool Object2DComponent::GetIsUpdated() const
	{
		return m_isUpdated;
	}

	void Object2DComponent::SetIsUpdated(bool value)
	{
		m_isUpdated = value;
	}

	bool Object2DComponent::GetIsAlive() const
	{
		return m_isAlive;
	}

	void Object2DComponent::Vanish()
	{
		m_isAlive = false;
	}
}