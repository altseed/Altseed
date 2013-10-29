#include <list>
#include "ace.Object2DBase.h"
using namespace std;

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Object2DBase::Object2DBase()
		: m_owner(nullptr)
		, m_children(list<Object2DBasePtr>())
		, m_components(map<astring, ComponentPtr>())
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Object2DBase::~Object2DBase()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Object2DBase::Start()
	{
		OnStart();
	}

	void Object2DBase::Update()
	{
		OnUpdate();
		for (auto& x : m_components)
		{
			x.second->Update();
		}
	}

	void Object2DBase::SetLayer(Layer2D* layer)
	{
		m_owner = layer;
	}

#pragma region Get/Set
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Layer2D* Object2DBase::GetLayer() const
	{
		return m_owner;
	}

	Vector2DF Object2DBase::GetPosition() const
	{
		return GetCoreObject()->GetPosition();
	}

	void Object2DBase::SetPosition(Vector2DF value)
	{
		GetCoreObject()->SetPosition(value);
	}

	Vector2DF Object2DBase::GetGlobalPosition()
	{
		return GetCoreObject()->GetGlobalPosition();
	}

	float Object2DBase::GetAngle() const
	{
		return GetCoreObject()->GetAngle();
	}

	void Object2DBase::SetAngle(float value)
	{
		GetCoreObject()->SetAngle(value);
	}

	Vector2DF Object2DBase::GetScale() const
	{
		return GetCoreObject()->GetScale();
	}

	void Object2DBase::SetScale(Vector2DF value)
	{
		GetCoreObject()->SetScale(value);
	}

#pragma endregion

	void Object2DBase::AddChild(const Object2DBasePtr& child, eChildMode mode)
	{
		GetCoreObject()->AddChild(*(child->GetCoreObject()), mode);
		m_children.push_back(child);
	}

	void Object2DBase::RemoveChild(const Object2DBasePtr& child)
	{
		GetCoreObject()->RemoveChild(*(child->GetCoreObject()));
		m_children.remove(child);
	}

	const std::list<Object2DBase::Object2DBasePtr>& Object2DBase::GetChildren() const
	{
		return m_children;
	}


	void Object2DBase::AddComponent(const ComponentPtr& component, astring key)
	{
		m_components[key] = component;
		component->SetOwner(this);
	}

	Object2DBase::ComponentPtr& Object2DBase::GetComponent(astring key)
	{
		return m_components[key];
	}

	void Object2DBase::RemoveComponent(astring key)
	{
		auto it = m_components.find(key);
		m_components.erase(it);
		it->second->SetOwner(nullptr);
	}
}