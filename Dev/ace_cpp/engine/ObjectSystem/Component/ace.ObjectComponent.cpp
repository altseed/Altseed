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

	Object2DBase* ObjectComponent::GetOwner() const
	{
		return m_object;
	}

	void ObjectComponent::SetOwner(Object2DBase* value)
	{
		m_object = value;
	}
}