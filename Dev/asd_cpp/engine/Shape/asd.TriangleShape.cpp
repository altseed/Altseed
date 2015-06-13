#include "asd.TriangleShape.h"

namespace asd
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	TriangleShape::TriangleShape()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateTriangleShape());
	}

	TriangleShape::~TriangleShape()
	{

	}

	std::shared_ptr<CoreShape> TriangleShape::GetCoreShape() const
	{
		return m_coreObject;
	}

	Vector2DF TriangleShape::GetPointByIndex(int index) const
	{
		return m_coreObject->GetPointByIndex(index);
	}

	void TriangleShape::SetPointByIndex(Vector2DF point, int index)
	{
		m_coreObject->SetPointByIndex(point, index);
	}

	Vector2DF TriangleShape::GetUVByIndex(int index) const
	{
		return m_coreObject->GetUVByIndex(index);
	}

	void TriangleShape::SetUVByIndex(Vector2DF uv, int index)
	{
		m_coreObject->SetUVByIndex(uv, index);
	}

	ShapeType TriangleShape::GetShapeType() const
	{
		return ShapeType::TriangleShape;
	}
};