#include "ace.CoreTriangle_Imp.h"
#include "ace.CoreArc_Imp.h"

namespace ace
{
	CoreArc_Imp::CoreArc_Imp()
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

	int CoreArc_Imp::GetStartingCorner() const
	{
		return startingCorner;
	}

	void CoreArc_Imp::SetStartingCorner(int startingverticalAngle)
	{
		isNeededUpdating = true;
		this->startingCorner = startingverticalAngle;
	}

	int CoreArc_Imp::GetEndingCorner() const
	{
		return endingCorner;
	}

	void CoreArc_Imp::SetEndingCorner(int endingCorner)
	{
		isNeededUpdating = true;
		this->endingCorner = endingCorner;
	}


	int CoreArc_Imp::GetNumberOfCorners() const
	{
		return numberOfCorners;
	}

	void CoreArc_Imp::SetNumberOfCorners(int numberOfCorners)
	{
		isNeededUpdating = true;
		this->numberOfCorners = numberOfCorners;
	}

	ShapeType CoreArc_Imp::GetShapeType() const
	{
		return ShapeType::Arc;
	}

	Vector2DF CoreArc_Imp::GetPosition() const
	{
		return position;
	}

	void CoreArc_Imp::SetPosition(Vector2DF position)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		this->position = position;
	}


	float CoreArc_Imp::GetAngle() const
	{
		return angle;
	}

	void CoreArc_Imp::SetAngle(float angle)
	{
		isNeededUpdating = true;
		this->angle = angle;
	}

	float CoreArc_Imp::GetOuterDiameter() const
	{
		return outerDiameter;
	}

	void CoreArc_Imp::SetOuterDiameter(float outerDiameter)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		this->outerDiameter = outerDiameter;
	}

	float CoreArc_Imp::GetInnerDiameter() const
	{
		return innerDiameter;
	}

	void CoreArc_Imp::SetInnerDiameter(float innerDiameter)
	{
		isNeededUpdating = true;
		this->innerDiameter = innerDiameter;
	}


#if !SWIG
	void CoreArc_Imp::DivideToTriangles()
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

			auto triangle1 = new CoreTriangle_Imp();
			auto triangle2 = new CoreTriangle_Imp();

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

	void CoreArc_Imp::CalculateBoundingCircle()
	{
		boundingCircle = culling2d::Circle(culling2d::Vector2DF(position.X, position.Y), outerDiameter / 2.0f);
	}
#endif

};