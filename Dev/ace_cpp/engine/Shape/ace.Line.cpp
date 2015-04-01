#include "ace.Line.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	Line::Line()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateLine());
	}

	Line::~Line()
	{

	}

	std::shared_ptr<CoreShape> Line::GetCoreShape() const
	{
		return m_coreObject;
	}

	Vector2DF Line::GetStartingPosition() const
	{
		return m_coreObject->GetStartingPosition();
	}

	void Line::SetStartingPosition(Vector2DF startingPosition)
	{
		m_coreObject->SetStartingPosition(startingPosition);
	}

	Vector2DF Line::GetEndingPosition() const
	{
		return m_coreObject->GetEndingPosition();
	}

	void Line::SetEndingPosition(Vector2DF endingPosition)
	{
		m_coreObject->SetEndingPosition(endingPosition);
	}

	float Line::GetThickness() const
	{
		return m_coreObject->GetThickness();
	}

	void Line::SetThickness(float thickness)
	{
		m_coreObject->SetThickness(thickness);
	}

	ShapeType Line::GetShapeType() const
	{
		return ShapeType::Line;
	}
}