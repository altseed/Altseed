#include "ace.Rectangle.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	Rectangle::Rectangle()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateRectangle());
	}

	Rectangle::~Rectangle()
	{

	}

	std::shared_ptr<CoreShape> Rectangle::GetCoreShape() const
	{
		return m_coreObject;
	}

	RectF Rectangle::GetDrawingArea() const
	{
		return m_coreObject->GetDrawingArea();
	}

	void Rectangle::SetDrawingArea(RectF drawingArea)
	{
		m_coreObject->SetDrawingArea(drawingArea);
	}

	RectF Rectangle::GetUV() const
	{
		return m_coreObject->GetUV();
	}

	void Rectangle::SetUV(RectF uv)
	{
		m_coreObject->SetUV(uv);
	}

	float Rectangle::GetAngle() const
	{
		return m_coreObject->GetAngle();
	}

	void Rectangle::SetAngle(float angle)
	{
		m_coreObject->SetAngle(angle);
	}

	Vector2DF Rectangle::GetCenterPosition() const
	{
		return m_coreObject->GetCenterPosition();
	}

	void Rectangle::SetCenterPosition(Vector2DF centerPosition)
	{
		m_coreObject->SetCenterPosition(centerPosition);
	}

	ShapeType Rectangle::GetShapeType() const
	{
		return ShapeType::Rectangle;
	}
}