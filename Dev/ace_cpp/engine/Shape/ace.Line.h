#pragma once

#include "ace.Shape.h"

namespace ace
{
	class Line :public Shape
	{
	private:
		std::shared_ptr<CoreLine> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;
	public:
		Line();
		virtual ~Line();

		Vector2DF GetStartingPosition() const;
		void SetStartingPosition(Vector2DF startingPosition);

		Vector2DF GetEndingPosition() const;
		void SetEndingPosition(Vector2DF endingPosition);

		float GetThickness() const;
		void SetThickness(float thickness);

		ShapeType GetShapeType() const override;
	};
}