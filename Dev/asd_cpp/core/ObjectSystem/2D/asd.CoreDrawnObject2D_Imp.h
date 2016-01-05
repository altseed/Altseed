#pragma once
#include "asd.CoreObject2D_Imp.h"
#include "asd.CoreDrawnObject2D.h"

namespace asd
{
	class CoreDrawnObject2D_Imp
		: public CoreObject2D_Imp
		// : public CoreDrawnObject2D  ïHå^åpè≥ñhé~ÇÃÇΩÇﬂÉRÉÅÉìÉgÉAÉEÉg
	{
	private:
		Color m_color;
		int m_drawingPriority;

	public:
		CoreDrawnObject2D_Imp(Graphics_Imp* graphics);

		Color GetColor() const;
		void SetColor(Color value);

		int GetDrawingPriority() const;
		void SetDrawingPriority(int value);

		Color GetAbsoluteColor() const;
		int GetAbsoluteDrawingPriority() const;
	};
}