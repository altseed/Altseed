#include "asd.SceneComponent.h"

namespace asd
{
	SceneComponent::SceneComponent()
		: m_scene(nullptr)
		, m_isUpdated(true)
		, m_isAlive(true)
	{
	}

	void SceneComponent::Update()
	{
		if (m_isUpdated)
		{
			OnUpdate();
		}
	}

	Scene* SceneComponent::GetOwner() const
	{
		return m_scene;
	}

	void SceneComponent::SetOwner(Scene* value)
	{
		m_scene = value;
	}

	bool SceneComponent::GetIsUpdated() const
	{
		return m_isUpdated;
	}

	void SceneComponent::SetIsUpdated(bool value)
	{
		m_isUpdated = value;
	}

	bool SceneComponent::GetIsAlive() const
	{
		return m_isAlive;
	}

	void SceneComponent::Vanish()
	{
		m_isAlive = false;
	}
}