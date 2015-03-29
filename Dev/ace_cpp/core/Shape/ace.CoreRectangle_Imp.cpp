#include "ace.CoreRectangle_Imp.h"

namespace ace
{

	CoreRectangle_Imp::CoreRectangle_Imp()
		:drawingArea(RectF())
		, uv(RectF())
		, angle(0)
	{

	}

	RectF CoreRectangle_Imp::GetDrawingArea() const
	{
		return drawingArea;
	}

	void CoreRectangle_Imp::SetDrawingArea(RectF drawingArea)
	{
		this->drawingArea = drawingArea;
	}

	RectF CoreRectangle_Imp::GetUV() const
	{
		return uv;
	}

	void CoreRectangle_Imp::SetUV(RectF uv)
	{
		this->uv = uv;
	}

	float CoreRectangle_Imp::GetAngle() const
	{
		return angle;
	}

	void CoreRectangle_Imp::SetAngle(float angle)
	{
		this->angle = angle;
	}

	ShapeType CoreRectangle_Imp::GetShapeType() const
	{
		return ShapeType::Rectangle;
	}
}
