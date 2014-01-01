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
		, m_children(list<Object2DPtr>())
		, m_components(map<astring, ComponentPtr>())
		, m_isUpdated(true)
		, m_isDrawn(true)
		, m_isAlive(true)
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

	bool Object2D::GetIsAlive() const
	{
		return m_isAlive;
	}

	void Object2D::Vanish()
	{
		m_isAlive = false;
	}


	Layer2D* Object2D::GetLayer() const
	{
		return m_owner;
	}

	void Object2D::SetLayer(Layer2D* layer)
	{
		m_owner = layer;
	}

	void Object2D::AddChild(const Object2DPtr& child, eChildMode mode)
	{
		GetCoreObject()->AddChild(*(child->GetCoreObject()), mode);
		m_children.push_back(child);
	}

	void Object2D::RemoveChild(const Object2DPtr& child)
	{
		GetCoreObject()->RemoveChild(*(child->GetCoreObject()));
		m_children.remove(child);
	}

	const std::list<Object2D::Object2DPtr>& Object2D::GetChildren() const
	{
		return m_children;
	}

	void Object2D::AddComponent(const ComponentPtr& component, astring key)
	{
		m_components[key] = component;
		component->SetOwner(this);
	}

	Object2D::ComponentPtr& Object2D::GetComponent(astring key)
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