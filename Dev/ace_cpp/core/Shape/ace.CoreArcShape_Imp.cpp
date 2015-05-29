#include "ace.CoreTriangleShape_Imp.h"
#include "ace.CoreArcShape_Imp.h"
#include <Box2D/Box2D.h>

namespace ace
{
	CoreArcShape_Imp::CoreArcShape_Imp()
		:
		position(Vector2DF())
		, startingCorner(0)
		, endingCorner(0)
		, innerDiameter(0)
		, outerDiameter(0)
		, numberOfCorners(0)
		, angle(0)
	{

	}

	int CoreArcShape_Imp::GetStartingCorner() const
	{
		return startingCorner;
	}

	void CoreArcShape_Imp::SetStartingCorner(int startingverticalAngle)
	{
		isNeededUpdating = true;
		isNeededCalcCollisions = true;
		this->startingCorner = startingverticalAngle;
	}

	int CoreArcShape_Imp::GetEndingCorner() const
	{
		return endingCorner;
	}

	void CoreArcShape_Imp::SetEndingCorner(int endingCorner)
	{
		isNeededUpdating = true;
		isNeededCalcCollisions = true;
		this->endingCorner = endingCorner;
	}


	int CoreArcShape_Imp::GetNumberOfCorners() const
	{
		return numberOfCorners;
	}

	void CoreArcShape_Imp::SetNumberOfCorners(int numberOfCorners)
	{
		isNeededUpdating = true;
		isNeededCalcCollisions = true;
		this->numberOfCorners = numberOfCorners;
	}

	ShapeType CoreArcShape_Imp::GetShapeType() const
	{
		return ShapeType::ArcShape;
	}

	Vector2DF CoreArcShape_Imp::GetPosition() const
	{
		return position;
	}

	void CoreArcShape_Imp::SetPosition(Vector2DF position)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		isNeededCalcCollisions = true;
		this->position = position;
	}


	float CoreArcShape_Imp::GetAngle() const
	{
		return angle;
	}

	void CoreArcShape_Imp::SetAngle(float angle)
	{
		isNeededUpdating = true;
		isNeededCalcCollisions = true;
		this->angle = angle;
	}

	float CoreArcShape_Imp::GetOuterDiameter() const
	{
		return outerDiameter;
	}

	void CoreArcShape_Imp::SetOuterDiameter(float outerDiameter)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		isNeededCalcCollisions = true;
		this->outerDiameter = outerDiameter;
	}

	float CoreArcShape_Imp::GetInnerDiameter() const
	{
		return innerDiameter;
	}

	void CoreArcShape_Imp::SetInnerDiameter(float innerDiameter)
	{
		isNeededUpdating = true;
		isNeededCalcCollisions = true;
		this->innerDiameter = innerDiameter;
	}


#if !SWIG
	void CoreArcShape_Imp::DivideToTriangles()
	{
		if (numberOfCorners < 3) return;

		const float radInc = 360.0 / numberOfCorners;

		const float outerRadius = outerDiameter / 2;
		const float innerRadius = innerDiameter / 2;

		float currentPosDeg = angle - 90 + startingCorner*radInc;
		float currentUVDeg = -90 + startingCorner*radInc;

		Vector2DF baseVector(0, -1);
		baseVector.SetDegree(currentPosDeg);

		Vector2DF uvCenter = { 0.5, 0.5 };

		Vector2DF uvVector = { 0, -0.5 };

		float ratio = innerDiameter / outerDiameter;

		int endcorner = endingCorner;
		while (endcorner < startingCorner) endcorner += numberOfCorners;

		for (int i = 0; i < endcorner-startingCorner; ++i)
		{
			Vector2DF currentPosVector = baseVector;
			currentPosVector.SetDegree(currentPosDeg);

			Vector2DF nextPosVector = currentPosVector;
			auto nextPosDeg = nextPosVector.GetDegree();
			nextPosDeg += radInc;
			nextPosVector.SetDegree(nextPosDeg);

			Vector2DF currentUVVector = uvVector;
			currentUVVector.SetDegree(currentUVDeg);

			Vector2DF nextUVVector = currentUVVector;
			auto nextUVDeg = nextUVVector.GetDegree();
			nextUVDeg += radInc;
			nextUVVector.SetDegree(nextUVDeg);


			std::array<Vector2DF, 4> vertexes = { position + currentPosVector*outerRadius, position + nextPosVector*outerRadius, position + nextPosVector*innerRadius, position + currentPosVector*innerRadius };
			std::array<Vector2DF, 4> uvs = { uvCenter + currentUVVector, uvCenter + nextUVVector, uvCenter + nextUVVector*ratio, uvCenter + currentUVVector*ratio };

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

			currentPosDeg += radInc;
			currentUVDeg += radInc;
		}
	}

	void CoreArcShape_Imp::CalculateBoundingCircle()
	{
		boundingCircle = culling2d::Circle(culling2d::Vector2DF(position.X, position.Y), outerDiameter / 2.0f);
	}

	void CoreArcShape_Imp::CalcCollisions()
	{
		if (numberOfCorners < 3) return;

		const float radInc = 360.0 / numberOfCorners;

		const float outerRadius = outerDiameter / 2;
		const float innerRadius = innerDiameter / 2;

		float currentPosDeg = angle - 90 + startingCorner*radInc;

		Vector2DF baseVector(0, -1);
		baseVector.SetDegree(currentPosDeg);

		float ratio = innerDiameter / outerDiameter;

		int endcorner = endingCorner;
		while (endcorner < startingCorner) endcorner += numberOfCorners;

		for (int i = 0; i < endcorner - startingCorner; ++i)
		{
			Vector2DF currentPosVector = baseVector;
			currentPosVector.SetDegree(currentPosDeg);

			Vector2DF nextPosVector = currentPosVector;
			auto nextPosDeg = nextPosVector.GetDegree();
			nextPosDeg += radInc;
			nextPosVector.SetDegree(nextPosDeg);

			std::array<Vector2DF, 4> vertexes = { position + currentPosVector*outerRadius, position + nextPosVector*outerRadius, position + nextPosVector*innerRadius, position + currentPosVector*innerRadius };

			auto polygon = new b2PolygonShape();

			std::vector<b2Vec2> polyPoints;
			for (auto vertex : vertexes)
			{
				polyPoints.push_back(b2Vec2(vertex.X, vertex.Y));
			}

			polygon->Set(polyPoints.data(), polyPoints.size());

			collisionShapes.push_back(polygon);

			currentPosDeg += radInc;
		}
	}
#endif

};