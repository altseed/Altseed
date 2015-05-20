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

	std::shared_ptr<CoreShape> PolygonShape::GetCoreShape() const
	{
		return m_coreObject;
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