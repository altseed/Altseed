#pragma once

#include "ace.Shape.h"

namespace ace
{
	class Arc :public Shape
	{
	private:
		std::shared_ptr<CoreArc> m_coreObject;
	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;

	public:
		Arc();
		virtual ~Arc();

		Vector2DF GetPosition() const;
		void SetPosition(Vector2DF position);

		float GetOuterDiameter() const;
		void SetOuterDiamater(float outerDiameter);

		float GetInnerDiameter() const;
		void SetInnerDiamater(float innerDiameter);

		float GetAngle() const;
		void SetAngle(float angle);

		int GetNumberOfCorners() const;
		void SetNumberOfCorners(int numberOfCorners);

		int GetStartingCorner() const;
		void SetStartingCorner(int startingCorner);

		int GetEndingCorner() const;
		void SetEndingCorner(int endingCorner);

		ShapeType GetShapeType() const override;
	};
}