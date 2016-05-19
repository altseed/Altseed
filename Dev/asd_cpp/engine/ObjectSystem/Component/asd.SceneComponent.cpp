#include "asd.SceneComponent.h"
#include "../asd.Scene.h"
#include "../Registration/asd.EventToDisposeContent.h"
using namespace std;

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

	astring SceneComponent::GetKey() const
	{
		return m_key;
	}

	void SceneComponent::SetKey(astring value)
	{
		m_key = value;
	}

	bool SceneComponent::GetIsAlive() const
	{
		return m_isAlive;
	}

	void SceneComponent::Dispose()
	{
		Engine::m_changesToCommit.push(make_shared<EventToDisposeContent>(shared_from_this()));
	}

	void SceneComponent::DisposeImmediately()
	{
		m_isAlive = false;
		m_scene->ImmediatelyRemoveComponent(GetKey());
	}
}