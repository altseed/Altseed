#include "ace.CoreTriangle_Imp.h"

namespace ace
{
	CoreTriangle_Imp::CoreTriangle_Imp()
		:points()
		, uvs()
	{

	}

	Vector2DF CoreTriangle_Imp::GetPointByIndex(int index) const
	{
		return points[index];
	}

	void CoreTriangle_Imp::SetPointByIndex(Vector2DF point, int index)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		points[index] = point;
	}

	Vector2DF CoreTriangle_Imp::GetUVByIndex(int index) const
	{
		return uvs[index];
	}

	void CoreTriangle_Imp::SetUVByIndex(Vector2DF uv, int index)
	{
		isNeededUpdating = true;
		uvs[index] = uv;
	}

	ShapeType CoreTriangle_Imp::GetShapeType() const
	{
		return ShapeType::Triangle;
	}

#if !SWIG
	void CoreTriangle_Imp::DivideToTriangles()
	{
		//SafeAddRef(this);
		triangles.push_back(this);
	}

	void CoreTriangle_Imp::CalculateBoundingCircle()
	{
		auto center = (points[0] + points[1] + points[2]) / 3.0f;
		auto radius = 0.0f;

		for (auto point : points)
		{
			radius = Max(radius, (center - point).GetLength());
		}

		boundingCircle = culling2d::Circle(culling2d::Vector2DF(center.X, center.Y), radius);
	}

#endif

};