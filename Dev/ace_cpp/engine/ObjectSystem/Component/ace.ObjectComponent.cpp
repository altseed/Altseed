#include "ace.ObjectComponent.h"

namespace ace
{
	ObjectComponent::ObjectComponent()
		: m_object(nullptr)
	{
	}

	void ObjectComponent::Update()
	{
		OnUpdate();
	}

	Object2D* ObjectComponent::GetOwner() const
	{
		return m_object;
	}

	void ObjectComponent::SetOwner(Object2D* value)
	{
		m_object = value;
	}
}