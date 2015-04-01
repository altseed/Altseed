#include "ace.Circle.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	Circle::Circle()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateCircle());
	}
	
	Circle::~Circle()
	{

	}

	std::shared_ptr<CoreShape> Circle::GetCoreShape() const
	{
		return m_coreObject;
	}

	Vector2DF Circle::GetPosition() const
	{
		return m_coreObject->GetPosition();
	}

	void Circle::SetPosition(Vector2DF position)
	{
		m_coreObject->SetPosition(position);
	}

	float Circle::GetOuterDiameter() const
	{
		return m_coreObject->GetOuterDiameter();
	}

	void Circle::SetOuterDiamater(float outerDiameter)
	{
		m_coreObject->SetOuterDiamater(outerDiameter);
	}

	float Circle::GetInnerDiameter() const
	{
		return m_coreObject->GetInnerDiameter();
	}

	void Circle::SetInnerDiamater(float innerDiameter)
	{
		m_coreObject->SetInnerDiamater(innerDiameter);
	}

	float Circle::GetAngle() const
	{
		return m_coreObject->GetAngle();
	}

	void Circle::SetAngle(float angle)
	{
		m_coreObject->SetAngle(angle);
	}

	int Circle::GetNumberOfCorners() const
	{
		return m_coreObject->GetNumberOfCorners();
	}

	void Circle::SetNumberOfCorners(int numberOfCorners)
	{
		m_coreObject->SetNumberOfCorners(numberOfCorners);
	}

	ShapeType Circle::GetShapeType() const
	{
		return ShapeType::Circle;
	}
}