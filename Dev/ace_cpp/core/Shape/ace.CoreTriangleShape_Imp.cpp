#include "ace.CoreTriangleShape_Imp.h"

namespace ace
{
	CoreTriangleShape_Imp::CoreTriangleShape_Imp()
		:points()
		, uvs()
	{

	}

	Vector2DF CoreTriangleShape_Imp::GetPointByIndex(int index) const
	{
		return points[index];
	}

	void CoreTriangleShape_Imp::SetPointByIndex(Vector2DF point, int index)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		isNeededCalcCollisions = true;
		points[index] = point;
	}

	Vector2DF CoreTriangleShape_Imp::GetUVByIndex(int index) const
	{
		return uvs[index];
	}

	void CoreTriangleShape_Imp::SetUVByIndex(Vector2DF uv, int index)
	{
		isNeededUpdating = true;
		uvs[index] = uv;
	}

	ShapeType CoreTriangleShape_Imp::GetShapeType() const
	{
		return ShapeType::TriangleShape;
	}

#if !SWIG
	void CoreTriangleShape_Imp::DivideToTriangles()
	{
		//SafeAddRef(this);
		CoreTriangleShape_Imp* triangle = new CoreTriangleShape_Imp();

		for (int i = 0; i < 3; ++i)
		{
			triangle->SetPointByIndex(points[i], i);
			triangle->SetUVByIndex(uvs[i], i);
		}

		triangles.push_back(triangle);
	}

	void CoreTriangleShape_Imp::CalculateBoundingCircle()
	{
		auto center = (points[0] + points[1] + points[2]) / 3.0f;
		auto radius = 0.0f;

		for (auto point : points)
		{
			radius = Max(radius, (center - point).GetLength());
		}

		boundingCircle = culling2d::Circle(culling2d::Vector2DF(center.X, center.Y), radius);
	}

	void CoreTriangleShape_Imp::CalcCollisions()
	{
		auto triangle = new b2PolygonShape();
		
		std::vector<b2Vec2> triPoints;
		for (int i = 0; i < 3; ++i)
		{
			triPoints.push_back(b2Vec2(points[i].X, points[i].Y));
		}

		triangle->Set(triPoints.data(), 3);

		collisionShapes.push_back(triangle);
	}
#endif

};