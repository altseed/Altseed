
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
	{

	}

	Object3D::~Object3D()
	{

	}

	Layer3D* Object3D::GetLayer() const
	{
		return m_owner;
	}

	Vector3DF Object3D::GetPosition() const
	{
		return m_commonObject->GetPosition();
	}
}