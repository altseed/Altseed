#include "ace.ArcShape.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	ArcShape::ArcShape()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateArcShape());
	}

	ArcShape::~ArcShape()
	{

	}

	std::shared_ptr<CoreShape> ArcShape::GetCoreShape() const
	{
		return m_coreObject;
	}

	Vector2DF ArcShape::GetPosition() const
	{
		return m_coreObject->GetPosition();
	}

	void ArcShape::SetPosition(Vector2DF position)
	{
		m_coreObject->SetPosition(position);
	}

	float ArcShape::GetOuterDiameter() const
	{
		return m_coreObject->GetOuterDiameter();
	}

	void ArcShape::SetOuterDiameter(float outerDiameter)
	{
		m_coreObject->SetOuterDiameter(outerDiameter);
	}

	float ArcShape::GetInnerDiameter() const
	{
		return m_coreObject->GetInnerDiameter();
	}

	void ArcShape::SetInnerDiameter(float innerDiameter)
	{
		m_coreObject->SetInnerDiameter(innerDiameter);
	}

	float ArcShape::GetAngle() const
	{
		return m_coreObject->GetAngle();
	}

	void ArcShape::SetAngle(float angle)
	{
		m_coreObject->SetAngle(angle);
	}

	int ArcShape::GetNumberOfCorners() const
	{
		return m_coreObject->GetNumberOfCorners();
	}

	void ArcShape::SetNumberOfCorners(int numberOfCorners)
	{
		m_coreObject->SetNumberOfCorners(numberOfCorners);
	}

	ShapeType ArcShape::GetShapeType() const
	{
		return ShapeType::ArcShape;
	}

	int ArcShape::GetStartingCorner() const
	{
		return m_coreObject->GetStartingCorner();
	}

	void ArcShape::SetStartingCorner(int startingCorner)
	{
		m_coreObject->SetStartingCorner(startingCorner);
	}

	int ArcShape::GetEndingCorner() const
	{
		return m_coreObject->GetEndingCorner();
	}

	void ArcShape::SetEndingCorner(int endingCorner)
	{
		m_coreObject->SetEndingCorner(endingCorner);
	}
}