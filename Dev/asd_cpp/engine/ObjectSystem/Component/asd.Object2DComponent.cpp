#include "asd.Object2DComponent.h"
#include "../2D/asd.Object2D.h"
#include "../Registration/asd.EventToDisposeContent.h"

using namespace std;

namespace asd
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

	astring Object2DComponent::GetKey() const
	{
		return m_key;
	}

	void Object2DComponent::SetKey(astring value)
	{
		m_key = value;
	}


	bool Object2DComponent::GetIsAlive() const
	{
		return m_isAlive;
	}

	void Object2DComponent::Dispose()
	{
		Engine::m_changesToCommit.push(make_shared<EventToDisposeContent>(shared_from_this()));
	}

	void Object2DComponent::DisposeImmediately()
	{
		m_isAlive = false;
		m_owner->ImmediatelyRemoveComponent(GetKey());
	}
}