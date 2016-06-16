#include "asd.Layer2DComponent.h"
#include "../2D/asd.Layer2D.h"
#include "../asd.Layer.h"
#include "../Registration/asd.EventToDisposeContent.h"
using namespace std;

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

	void Layer2DComponent::RaiseOnAdded()
	{
		if (m_isUpdated)
		{
			OnLayerAdded();
		}
	}

	void Layer2DComponent::RaiseOnRemoved()
	{
		if (m_isUpdated)
		{
			OnLayerRemoved();
		}
	}

	void Layer2DComponent::RaiseOnDisposed()
	{
		if (m_isUpdated)
		{
			OnLayerDisposed();
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

	astring Layer2DComponent::GetKey() const
	{
		return m_key;
	}

	void Layer2DComponent::SetKey(astring value)
	{
		m_key = value;
	}


	bool Layer2DComponent::GetIsAlive() const
	{
		return m_isAlive;
	}

	void Layer2DComponent::Dispose()
	{
		Engine::m_changesToCommit.push(make_shared<EventToDisposeContent>(shared_from_this()));
	}

	void Layer2DComponent::DisposeImmediately()
	{
		m_isAlive = false;
		m_layer->ImmediatelyRemoveComponent(m_key);
	}
}