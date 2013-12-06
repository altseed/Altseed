
#include "ace.Object3D.h"

namespace ace
{
	void Object3D::Start()
	{
		OnStart();
	}

	void Object3D::SetLayer(Layer3D* layer)
	{
		m_owner = layer;
	}

	CoreObject3D* Object3D::GetCoreObject() const
	{
		return m_commonObject;
	}

	Object3D::Object3D()
		: m_owner(nullptr)
		, m_commonObject(nullptr)
		, m_isUpdated(true)
		, m_isDrawn(true)
		, m_isAlive(true)
	{

	}

	Object3D::~Object3D()
	{

	}

	Layer3D* Object3D::GetLayer() const
	{
		return m_owner;
	}


	bool Object3D::GetIsUpdated() const
	{
		return m_isUpdated;
	}

	void Object3D::SetIsUpdated(bool value)
	{
		m_isUpdated = value;
	}

	bool Object3D::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void Object3D::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
		GetCoreObject()->SetIsDrawn(value);
	}

	bool Object3D::GetIsAlive() const
	{
		return m_isAlive;
	}

	void Object3D::Vanish()
	{
		m_isAlive = false;
	}

	Vector3DF Object3D::GetPosition() const
	{
		return m_commonObject->GetPosition();
	}
}