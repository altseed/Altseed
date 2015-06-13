#include "asd.RectangleShape.h"

namespace asd
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	RectangleShape::RectangleShape()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateRectangleShape());
	}

	RectangleShape::~RectangleShape()
	{

	}

	std::shared_ptr<CoreShape> RectangleShape::GetCoreShape() const
	{
		return m_coreObject;
	}

	RectF RectangleShape::GetDrawingArea() const
	{
		return m_coreObject->GetDrawingArea();
	}

	void RectangleShape::SetDrawingArea(RectF drawingArea)
	{
		m_coreObject->SetDrawingArea(drawingArea);
	}

	RectF RectangleShape::GetUV() const
	{
		return m_coreObject->GetUV();
	}

	void RectangleShape::SetUV(RectF uv)
	{
		m_coreObject->SetUV(uv);
	}

	float RectangleShape::GetAngle() const
	{
		return m_coreObject->GetAngle();
	}

	void RectangleShape::SetAngle(float angle)
	{
		m_coreObject->SetAngle(angle);
	}

	Vector2DF RectangleShape::GetCenterPosition() const
	{
		return m_coreObject->GetCenterPosition();
	}

	void RectangleShape::SetCenterPosition(Vector2DF centerPosition)
	{
		m_coreObject->SetCenterPosition(centerPosition);
	}

	ShapeType RectangleShape::GetShapeType() const
	{
		return ShapeType::RectangleShape;
	}
}