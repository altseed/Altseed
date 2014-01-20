#include "ace.ObjectInfo2D.h"

namespace ace
{
	ObjectInfo2D::ObjectInfo2D()
		: m_layer(nullptr)
		, m_isDrawn(true)
		, m_isAlive(true)
	{
	}

	bool ObjectInfo2D::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void ObjectInfo2D::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
	}

	bool ObjectInfo2D::GetIsAlive() const
	{
		return m_isAlive;
	}

	void ObjectInfo2D::SetIsAlive(bool value)
	{
		m_isAlive = value;
	}

	void ObjectInfo2D::SetLayer(CoreLayer2D* layer)
	{
		m_layer = layer;
	}

	CoreLayer2D* ObjectInfo2D::GetLayer()
	{
		return m_layer;
	}
}