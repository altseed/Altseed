#include "ace.CoreCircle_Imp.h"
#include "ace.CoreTriangle_Imp.h"

namespace ace
{
	CoreCircle_Imp::CoreCircle_Imp()
		:position(Vector2DF(0,0))
		, radius(0)
		, verticalAngleNum(0)
	{

	}

	Vector2DF CoreCircle_Imp::GetPosition() const
	{
		return position;
	}

	void CoreCircle_Imp::SetPosition(Vector2DF position)
	{
		this->position = position;
	}

	float CoreCircle_Imp::GetRadius() const
	{
		return radius;
	}

	void CoreCircle_Imp::SetRadius(float radius)
	{
		this->radius = radius;
	}

	int CoreCircle_Imp::GetVerticalAngleNum() const
	{
		return verticalAngleNum;
	}

	void CoreCircle_Imp::SetVerticalAngleNum(int verticalAngleNum)
	{
		verticalAngleNum = verticalAngleNum;
	}

	ShapeType CoreCircle_Imp::GetShapeType() const
	{
		return ShapeType::Circle;
	}

#if !SWIG
	std::vector<CoreTriangle*> CoreCircle_Imp::GetDividedTriangles()
	{
		std::vector <CoreTriangle*> triangles;

		return triangles;
	}
#endif

};