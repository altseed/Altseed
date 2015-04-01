#pragma once

#include "ace.Shape.h"

namespace ace
{
	class Rectangle :public Shape
	{
	private:
		std::shared_ptr<CoreRectangle> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;
	public:
		Rectangle();
		virtual ~Rectangle();

		RectF GetDrawingArea() const;
		void SetDrawingArea(RectF drawingArea);

		RectF GetUV() const;
		void SetUV(RectF uv);

		float GetAngle() const;
		void SetAngle(float angle);

		Vector2DF GetCenterPosition() const;
		void SetCenterPosition(Vector2DF centerPosition);

		ShapeType GetShapeType() const override;
	};
}