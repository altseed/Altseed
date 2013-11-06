#include "ace.Object2DComponent.h"

namespace ace
{
	Object2DComponent::Object2DComponent()
		: m_object(nullptr)
	{
	}

	void Object2DComponent::Update()
	{
		OnUpdate();
	}

	Object2D* Object2DComponent::GetOwner() const
	{
		return m_object;
	}

	void Object2DComponent::SetOwner(Object2D* value)
	{
		m_object = value;
	}
}