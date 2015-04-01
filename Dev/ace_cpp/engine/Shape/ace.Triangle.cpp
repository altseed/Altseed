#include "ace.Triangle.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	Triangle::Triangle()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateTriangle());
	}

	Triangle::~Triangle()
	{

	}

	std::shared_ptr<CoreShape> Triangle::GetCoreShape() const
	{
		return m_coreObject;
	}

	Vector2DF Triangle::GetPointByIndex(int index) const
	{
		return m_coreObject->GetPointByIndex(index);
	}

	void Triangle::SetPointByIndex(Vector2DF point, int index)
	{
		m_coreObject->SetPointByIndex(point, index);
	}

	Vector2DF Triangle::GetUVByIndex(int index) const
	{
		return m_coreObject->GetUVByIndex(index);
	}

	void Triangle::SetUVByIndex(Vector2DF uv, int index)
	{
		m_coreObject->SetUVByIndex(uv, index);
	}

	ShapeType Triangle::GetShapeType() const
	{
		return ShapeType::Triangle;
	}
};