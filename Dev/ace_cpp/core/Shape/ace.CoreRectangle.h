#pragma once

#include "ace.CoreShape.h"
#include "../common/Math/ace.Vector2DF.h"
#include "../common/Math/ace.RectF.h"

namespace ace
{
	class CoreRectangle
		:public CoreShape
	{
	public:
		CoreRectangle(){}
		virtual ~CoreRectangle(){}

		virtual RectF GetDrawingArea() const = 0;
		virtual void SetDrawingArea(RectF drawingArea) = 0;

		virtual RectF GetUV() const = 0;
		virtual void SetUV(RectF uv) = 0;

		virtual float GetAngle() const = 0;
		virtual void SetAngle(float angle) = 0;
	};
};