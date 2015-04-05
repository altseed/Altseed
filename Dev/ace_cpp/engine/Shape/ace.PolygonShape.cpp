#include "ace.PolygonShape.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	PolygonShape::PolygonShape()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreatePolygonShape());
	}

	PolygonShape::~PolygonShape()
	{

	}

	void PolygonShape::AddVertex(Vector2DF vertex)
	{
		m_coreObject->AddVertex(vertex);
	}

	void PolygonShape::ClearVertexes()
	{
		m_coreObject->ClearVertexes();
	}

	int PolygonShape::GetVertexesNum() const
	{
		return m_coreObject->GetVertexesNum();
	}

	ShapeType PolygonShape::GetShapeType() const
	{
		return ShapeType::PolygonShape;
	}
}