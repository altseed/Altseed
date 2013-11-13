#include "ace.SceneComponent.h"

namespace ace
{
	SceneComponent::SceneComponent()
		: m_scene(nullptr)
	{
	}

	void SceneComponent::Update()
	{
		OnUpdate();
	}

	Scene* SceneComponent::GetOwner() const
	{
		return m_scene;
	}

	void SceneComponent::SetOwner(Scene* value)
	{
		m_scene = value;
	}
}