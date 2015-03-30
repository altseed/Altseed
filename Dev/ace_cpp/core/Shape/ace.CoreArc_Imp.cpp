#include "ace.CoreArc_Imp.h"
#include "ace.CoreTriangle_Imp.h"
namespace ace
{
	CoreArc_Imp::CoreArc_Imp()
		:
		position(Vector2DF())
		, radius(0)
		, staringVerticalAngle(0)
		, endingVerticalAngle(0)
		, verticalAngleNum(0)
	{

	}

	int CoreArc_Imp::GetStaringVerticalAngle() const
	{
		return staringVerticalAngle;
	}

	void CoreArc_Imp::SetStaringVerticalAngle(int staringverticalAngle)
	{
		staringVerticalAngle = staringverticalAngle;
	}

	int CoreArc_Imp::GetEndingVerticalAngle() const
	{
		return endingVerticalAngle;
	}

	void CoreArc_Imp::SetEndingVerticalAngle(int endingverticalAngle)
	{
		endingVerticalAngle = endingverticalAngle;
	}


	int CoreArc_Imp::GetVerticalAngleNum() const
	{
		return verticalAngleNum;
	}

	void CoreArc_Imp::SetVerticalAngleNum(int verticalangleNum)
	{
		verticalAngleNum = verticalangleNum;
	}

	ShapeType CoreArc_Imp::GetShapeType() const
	{
		return ShapeType::Arc;
	}

	Vector2DF CoreArc_Imp::GetPosition() const
	{
		return position;
	}

	void CoreArc_Imp::SetPosition(Vector2DF pos)
	{
		position = position;
	}


	float CoreArc_Imp::GetRadius() const
	{
		return radius;
	}

	void CoreArc_Imp::SetRadius(float rad)
	{
		radius = rad;
	}


#if !SWIG
	std::vector<CoreTriangle*> CoreArc_Imp::GetDividedTriangles()
	{
		std::vector <CoreTriangle*> triangles;

		return triangles;
	}
#endif

};