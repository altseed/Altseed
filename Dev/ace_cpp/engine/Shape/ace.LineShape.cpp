#include "ace.LineShape.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	LineShape::LineShape()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateLineShape());
	}

	LineShape::~LineShape()
	{

	}

	std::shared_ptr<CoreShape> LineShape::GetCoreShape() const
	{
		return m_coreObject;
	}

	Vector2DF LineShape::GetStartingPosition() const
	{
		return m_coreObject->GetStartingPosition();
	}

	void LineShape::SetStartingPosition(Vector2DF startingPosition)
	{
		m_coreObject->SetStartingPosition(startingPosition);
	}

	Vector2DF LineShape::GetEndingPosition() const
	{
		return m_coreObject->GetEndingPosition();
	}

	void LineShape::SetEndingPosition(Vector2DF endingPosition)
	{
		m_coreObject->SetEndingPosition(endingPosition);
	}

	float LineShape::GetThickness() const
	{
		return m_coreObject->GetThickness();
	}

	void LineShape::SetThickness(float thickness)
	{
		m_coreObject->SetThickness(thickness);
	}

	ShapeType LineShape::GetShapeType() const
	{
		return ShapeType::LineShape;
	}
}