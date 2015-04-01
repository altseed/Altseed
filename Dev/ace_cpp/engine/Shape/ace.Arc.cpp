#include "ace.Arc.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	Arc::Arc()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateArc());
	}

	Arc::~Arc()
	{

	}

	std::shared_ptr<CoreShape> Arc::GetCoreShape() const
	{
		return m_coreObject;
	}

	Vector2DF Arc::GetPosition() const
	{
		return m_coreObject->GetPosition();
	}

	void Arc::SetPosition(Vector2DF position)
	{
		m_coreObject->SetPosition(position);
	}

	float Arc::GetOuterDiameter() const
	{
		return m_coreObject->GetOuterDiameter();
	}

	void Arc::SetOuterDiamater(float outerDiameter)
	{
		m_coreObject->SetOuterDiamater(outerDiameter);
	}

	float Arc::GetInnerDiameter() const
	{
		return m_coreObject->GetInnerDiameter();
	}

	void Arc::SetInnerDiamater(float innerDiameter)
	{
		m_coreObject->SetInnerDiamater(innerDiameter);
	}

	float Arc::GetAngle() const
	{
		return m_coreObject->GetAngle();
	}

	void Arc::SetAngle(float angle)
	{
		m_coreObject->SetAngle(angle);
	}

	int Arc::GetNumberOfCorners() const
	{
		return m_coreObject->GetNumberOfCorners();
	}

	void Arc::SetNumberOfCorners(int numberOfCorners)
	{
		m_coreObject->SetNumberOfCorners(numberOfCorners);
	}

	ShapeType Arc::GetShapeType() const
	{
		return ShapeType::Arc;
	}

	int Arc::GetStartingCorner() const
	{
		return m_coreObject->GetStartingCorner();
	}

	void Arc::SetStartingCorner(int startingCorner)
	{
		m_coreObject->SetStartingCorner(startingCorner);
	}

	int Arc::GetEndingCorner() const
	{
		return m_coreObject->GetEndingCorner();
	}

	void Arc::SetEndingCorner(int endingCorner)
	{
		m_coreObject->SetEndingCorner(endingCorner);
	}
}