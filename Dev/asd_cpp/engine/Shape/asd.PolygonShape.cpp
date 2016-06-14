#include "asd.PolygonShape.h"

namespace asd
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

	bool PolygonShape::AddHole(std::shared_ptr<CoreShape> holeShape)
	{
		holeShapes.push_back(holeShape);
		return m_coreObject->AddHole(holeShape.get());
	}

	bool PolygonShape::RemoveHole(std::shared_ptr<CoreShape> holeShape)
	{
		holeShapes.erase(std::remove(holeShapes.begin(), holeShapes.end(), holeShape), holeShapes.end());
		return m_coreObject->RemoveHole(holeShape.get());
	}

	std::vector<std::shared_ptr<CoreShape>>& PolygonShape::GetHoleShapes()
	{
		return holeShapes;
	}
}