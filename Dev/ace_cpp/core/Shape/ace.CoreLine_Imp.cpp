#include "ace.CoreLine_Imp.h"

namespace ace
{
	CoreLine_Imp::CoreLine_Imp()
		:staringPoint(Vector2DF(0,0))
		, endingPoint(Vector2DF(0,0))
		, thickness(0)
	{

	}

	Vector2DF CoreLine_Imp::GetStartingPoint() const
	{
		return staringPoint;
	}

	void CoreLine_Imp::SetStartingPoint(Vector2DF startingPoint)
	{
		this->staringPoint = staringPoint;
	}

	Vector2DF CoreLine_Imp::GetEndingPoint() const
	{
		return endingPoint;
	}

	void CoreLine_Imp::SetEndingPoint(Vector2DF endingPoint)
	{
		this->endingPoint = endingPoint;
	}

	float CoreLine_Imp::GetThickness() const
	{
		return thickness;
	}

	void CoreLine_Imp::SetThickness(float thickness)
	{
		this->thickness = thickness;
	}

	ShapeType CoreLine_Imp::GetShapeType() const
	{
		return ShapeType::Line;
	}
};