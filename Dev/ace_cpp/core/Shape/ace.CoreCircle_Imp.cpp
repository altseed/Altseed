#include "ace.CoreCircle_Imp.h"
#include "ace.CoreTriangle_Imp.h"

namespace ace
{
	CoreCircle_Imp::CoreCircle_Imp()
		:position(Vector2DF(0,0))
		, innerDiameter(0)
		, outerDiameter(0)
		, numberOfCorners(0)
		, angle(0)
	{

	}

	Vector2DF CoreCircle_Imp::GetPosition() const
	{
		return position;
	}

	void CoreCircle_Imp::SetPosition(Vector2DF position)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		this->position = position;
	}

	float CoreCircle_Imp::GetAngle() const
	{
		return angle;
	}

	void CoreCircle_Imp::SetAngle(float angle)
	{
		isNeededUpdating = true;
		this->angle = angle;
	}

	float CoreCircle_Imp::GetOuterDiameter() const
	{
		return outerDiameter;
	}

	void CoreCircle_Imp::SetOuterDiamater(float outerDiameter)
	{
		isNeededUpdating = true;
		isNeededCalcBoundingCircle = true;
		this->outerDiameter = outerDiameter;
	}

	float CoreCircle_Imp::GetInnerDiameter() const
	{
		return innerDiameter;
	}

	void CoreCircle_Imp::SetInnerDiamater(float innerDiameter)
	{
		isNeededUpdating = true;
		this->innerDiameter = innerDiameter;
	}

	int CoreCircle_Imp::GetNumberOfCorners() const
	{
		return numberOfCorners;
	}

	void CoreCircle_Imp::SetNumberOfCorners(int numberOfCorners)
	{
		isNeededUpdating = true;
		this->numberOfCorners = numberOfCorners;
	}

	ShapeType CoreCircle_Imp::GetShapeType() const
	{
		return ShapeType::Circle;
	}

#if !SWIG
	void CoreCircle_Imp::DivideToTriangles()
	{
		if (numberOfCorners < 3) return;

		const float radInc = 360.0 / numberOfCorners;

		const float outerRadius = outerDiameter / 2;
		const float innerRadius = innerDiameter / 2;

		float currentPosDeg = angle - 90;
		float currentUVDeg = -90;

		Vector2DF baseVector(0, -1);
		baseVector.SetDegree(currentPosDeg);

		Vector2DF uvCenter = { 0.5, 0.5 };

		Vector2DF uvVector = { 0, -0.5 };

		float ratio = innerDiameter / outerDiameter;

		for (int i = 0; i < numberOfCorners; ++i)
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
			std::array<Vector2DF, 4> uvs = { uvCenter + uvVector, uvCenter + nextUVVector, uvCenter + nextUVVector*ratio, uvCenter + uvVector*ratio };

			auto triangle1 = new CoreTriangle_Imp();
			auto triangle2 = new CoreTriangle_Imp();

			for (int j = 0; j < 3; ++j)
			{
				triangle1->SetPointByIndex(vertexes[j], j);
				triangle1->SetUVByIndex(uvs[j], j);

				triangle2->SetPointByIndex(vertexes[j + 1], j + 1);
				triangle2->SetUVByIndex(uvs[j + 1], j + 1);
			}

			triangles.push_back(triangle1);
			triangles.push_back(triangle2);

			currentPosDeg += radInc;
			currentUVDeg += radInc;
		}
		
	}

	void CoreCircle_Imp::CalculateBoundingCircle()
	{
		boundingCircle = culling2d::Circle(culling2d::Vector2DF(position.X, position.Y), outerDiameter / 2.0f);
	}
#endif

};