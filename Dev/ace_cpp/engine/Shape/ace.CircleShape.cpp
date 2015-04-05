#include "ace.CircleShape.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	CircleShape::CircleShape()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateCircleShape());
	}
	
	CircleShape::~CircleShape()
	{

	}

	std::shared_ptr<CoreShape> CircleShape::GetCoreShape() const
	{
		return m_coreObject;
	}

	Vector2DF CircleShape::GetPosition() const
	{
		return m_coreObject->GetPosition();
	}

	void CircleShape::SetPosition(Vector2DF position)
	{
		m_coreObject->SetPosition(position);
	}

	float CircleShape::GetOuterDiameter() const
	{
		return m_coreObject->GetOuterDiameter();
	}

	void CircleShape::SetOuterDiameter(float outerDiameter)
	{
		m_coreObject->SetOuterDiameter(outerDiameter);
	}

	float CircleShape::GetInnerDiameter() const
	{
		return m_coreObject->GetInnerDiameter();
	}

	void CircleShape::SetInnerDiameter(float innerDiameter)
	{
		m_coreObject->SetInnerDiameter(innerDiameter);
	}

	float CircleShape::GetAngle() const
	{
		return m_coreObject->GetAngle();
	}

	void CircleShape::SetAngle(float angle)
	{
		m_coreObject->SetAngle(angle);
	}

	int CircleShape::GetNumberOfCorners() const
	{
		return m_coreObject->GetNumberOfCorners();
	}

	void CircleShape::SetNumberOfCorners(int numberOfCorners)
	{
		m_coreObject->SetNumberOfCorners(numberOfCorners);
	}

	ShapeType CircleShape::GetShapeType() const
	{
		return ShapeType::CircleShape;
	}
}