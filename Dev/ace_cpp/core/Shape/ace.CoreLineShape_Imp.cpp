#include "ace.CoreLineShape_Imp.h"
#include "ace.CoreTriangleShape_Imp.h"
#include <Box2D/Box2D.h>

namespace ace
{
	CoreLineShape_Imp::CoreLineShape_Imp()
		:staringPosition(Vector2DF(0, 0))
		, endingPosition(Vector2DF(0, 0))
		, thickness(0)
	{

	}

	Vector2DF CoreLineShape_Imp::GetStartingPosition() const
	{
		return staringPosition;
	}

	void CoreLineShape_Imp::SetStartingPosition(Vector2DF startingPosition)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		isNeededCalcCollisions = true;
		this->staringPosition = startingPosition;
	}

	Vector2DF CoreLineShape_Imp::GetEndingPosition() const
	{
		return endingPosition;
	}

	void CoreLineShape_Imp::SetEndingPosition(Vector2DF endingPosition)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		isNeededCalcCollisions = true;
		this->endingPosition = endingPosition;
	}

	float CoreLineShape_Imp::GetThickness() const
	{
		return thickness;
	}

	void CoreLineShape_Imp::SetThickness(float thickness)
	{
		isNeededCalcBoundingCircle = true;
		isNeededUpdating = true;
		isNeededCalcCollisions = true;
		this->thickness = thickness;
	}

	ShapeType CoreLineShape_Imp::GetShapeType() const
	{
		return ShapeType::LineShape;
	}

#if !SWIG
	void CoreLineShape_Imp::DivideToTriangles()
	{
		Vector2DF vector = endingPosition - staringPosition;

		if (vector.GetSquaredLength() == 0) return;

		auto binorm = vector;
		{
			auto deg = binorm.GetDegree();
			deg += 90;
			binorm.SetDegree(deg);
			binorm.Normalize();
		}

		auto halfThickness = thickness / 2;

		std::array<Vector2DF, 4> vertexes = { staringPosition + binorm*halfThickness, endingPosition + binorm*halfThickness, endingPosition - binorm*halfThickness, staringPosition - binorm*halfThickness };
		std::array<Vector2DF, 4> uvs = { Vector2DF(0, 0), Vector2DF(0, 0), Vector2DF(0, 0), Vector2DF(0, 0) };

		auto triangle1 = new CoreTriangleShape_Imp();
		auto triangle2 = new CoreTriangleShape_Imp();

		for (int j = 0; j < 3; ++j)
		{
			triangle1->SetPointByIndex(vertexes[j], j);
			triangle1->SetUVByIndex(uvs[j], j);

			triangle2->SetPointByIndex(vertexes[(j + 2) % 4], j);
			triangle2->SetUVByIndex(uvs[(j + 2) % 4], j);
		}

		triangles.push_back(triangle1);
		triangles.push_back(triangle2);
	}

	void CoreLineShape_Imp::CalculateBoundingCircle()
	{
		Vector2DF vector = endingPosition - staringPosition;

		if (vector.GetSquaredLength() == 0) return;

		auto binorm = vector;
		{
			auto deg = binorm.GetDegree();
			deg += 90;
			binorm.SetDegree(deg);
			binorm.Normalize();
		}

		auto halfThickness = thickness / 2;

		std::array<Vector2DF, 4> vertexes = { staringPosition + binorm*halfThickness, endingPosition + binorm*halfThickness, endingPosition - binorm*halfThickness, staringPosition - binorm*halfThickness };

		Vector2DF center = (vertexes[0] + vertexes[1] + vertexes[2] + vertexes[3]) / 4.0f;
		float radius = (vertexes[0] - center).GetLength();

		boundingCircle = culling2d::Circle(culling2d::Vector2DF(center.X, center.Y), radius);

	}

	void CoreLineShape_Imp::CalcCollisions()
	{
		Vector2DF vector = endingPosition - staringPosition;

		if (vector.GetSquaredLength() == 0) return;

		auto binorm = vector;
		{
			auto deg = binorm.GetDegree();
			deg += 90;
			binorm.SetDegree(deg);
			binorm.Normalize();
		}

		auto halfThickness = thickness / 2;

		std::array<Vector2DF, 4> vertexes = { staringPosition + binorm*halfThickness, endingPosition + binorm*halfThickness, endingPosition - binorm*halfThickness, staringPosition - binorm*halfThickness };

		auto polygon = new b2PolygonShape();

		std::vector<b2Vec2> polyPoints;
		for (auto vertex : vertexes)
		{
			polyPoints.push_back(b2Vec2(vertex.X, vertex.Y));
		}

		polygon->Set(polyPoints.data(), polyPoints.size());

		collisionShapes.push_back(polygon);
	}
#endif

};