#pragma once
#include "../common/Graphics/asd.Color.h"
#include "asd.CoreObject2D.h"

namespace asd
{
	class DrawnObject2D : public CoreObject2D
	{
	public:
		virtual Color GetColor() const = 0;
		virtual void SetColor(Color value) = 0;

		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int value) = 0;
	};
}