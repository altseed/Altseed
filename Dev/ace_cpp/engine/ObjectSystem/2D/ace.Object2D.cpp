#include <list>
#include "ace.Object2D.h"
using namespace std;

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Object2D::Object2D()
		: m_owner(nullptr)
		, m_children(list<Object2D::Ptr>())
		, m_components(map<astring, Object2DComponent::Ptr>())
		, m_isUpdated(true)
		, m_isDrawn(true)
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Object2D::~Object2D()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Object2D::Start()
	{
		OnStart();
	}

	void Object2D::UpdateComponents()
	{
		auto beVanished = vector<astring>();
		for (auto& x : m_components)
		{
			x.second->Update();
			if (!x.second->GetIsAlive())
			{
				beVanished.push_back(x.first);
			}
		}

		for (auto& x : beVanished)
		{
			RemoveComponent(x);
		}
	}

	void Object2D::Update()
	{
		if (!m_isUpdated)
		{
			return;
		}

		OnUpdate();
		UpdateComponents();
	}

	void Object2D::OnStart()
	{
	}

	void Object2D::OnUpdate()
	{
	}

	void Object2D::OnDrawAdditionally()
	{
		DrawSpriteAdditionally();
	}

	void Object2D::DrawSpriteAdditionally()
	{

	}


	bool Object2D::GetIsAlive() const
	{
		return GetCoreObject()->GetIsAlive();
	}

	void Object2D::Vanish()
	{
		GetCoreObject()->SetIsAlive(false);
	}

	Layer2D* Object2D::GetLayer() const
	{
		return m_owner;
	}

	void Object2D::SetLayer(Layer2D* layer)
	{
		m_owner = layer;
	}

	void Object2D::AddChild(const Object2D::Ptr& child, eChildMode mode)
	{
		GetCoreObject()->AddChild(*(child->GetCoreObject()), mode);
		m_children.push_back(child);
	}

	void Object2D::RemoveChild(const Object2D::Ptr& child)
	{
		GetCoreObject()->RemoveChild(*(child->GetCoreObject()));
		m_children.remove(child);
	}

	const std::list<Object2D::Ptr>& Object2D::GetChildren() const
	{
		return m_children;
	}

	void Object2D::AddComponent(const Object2DComponent::Ptr& component, astring key)
	{
		m_components[key] = component;
		component->SetOwner(this);
	}

	Object2DComponent::Ptr& Object2D::GetComponent(astring key)
	{
		return m_components[key];
	}

	void Object2D::RemoveComponent(astring key)
	{
		auto it = m_components.find(key);
		it->second->SetOwner(nullptr);
		m_components.erase(it);
	}

#pragma region Get/Set

	bool Object2D::GetIsUpdated() const
	{
		return m_isUpdated;
	}

	void Object2D::SetIsUpdated(bool value)
	{
		m_isUpdated = value;
	}

	bool Object2D::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void Object2D::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
		GetCoreObject()->SetIsDrawn(value);
	}

	Vector2DF Object2D::GetPosition() const
	{
		return GetCoreObject()->GetPosition();
	}

	void Object2D::SetPosition(Vector2DF value)
	{
		GetCoreObject()->SetPosition(value);
	}

	Vector2DF Object2D::GetGlobalPosition()
	{
		return GetCoreObject()->GetGlobalPosition();
	}

	float Object2D::GetAngle() const
	{
		return GetCoreObject()->GetAngle();
	}

	void Object2D::SetAngle(float value)
	{
		GetCoreObject()->SetAngle(value);
	}

	Vector2DF Object2D::GetScale() const
	{
		return GetCoreObject()->GetScale();
	}

	void Object2D::SetScale(Vector2DF value)
	{
		GetCoreObject()->SetScale(value);
	}

#pragma endregion
}