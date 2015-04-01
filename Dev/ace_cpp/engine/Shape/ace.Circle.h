#pragma once

#include "ace.Shape.h"

namespace ace
{
	class Circle :public Shape
	{
	private:
		std::shared_ptr<CoreCircle> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;

	public:
		Circle();
		virtual ~Circle();

		Vector2DF GetPosition() const;
		void SetPosition(Vector2DF position);

		float GetOuterDiameter() const;
		void SetOuterDiameter(float outerDiameter);

		float GetInnerDiameter() const;
		void SetInnerDiameter(float innerDiameter);

		float GetAngle() const;
		void SetAngle(float angle);

		int GetNumberOfCorners() const;
		void SetNumberOfCorners(int numberOfCorners);

		ShapeType GetShapeType() const;
	};
}