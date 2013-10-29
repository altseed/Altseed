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

	Scene* SceneComponent::GetScene() const
	{
		return m_scene;
	}

	void SceneComponent::SetScene(Scene* value)
	{
		m_scene = value;
	}
}