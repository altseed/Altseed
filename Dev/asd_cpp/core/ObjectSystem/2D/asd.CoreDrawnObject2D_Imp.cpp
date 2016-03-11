#include "asd.CoreDrawnObject2D_Imp.h"

namespace asd
{
	CoreDrawnObject2D_Imp::CoreDrawnObject2D_Imp(Graphics_Imp* graphics)
		: CoreObject2D_Imp(graphics)
		, m_color(Color(255, 255, 255, 255))
		, m_drawingPriority(0)
	{
	}

	Color CoreDrawnObject2D_Imp::GetColor() const
	{
		return m_color;
	}

	void CoreDrawnObject2D_Imp::SetColor(Color value)
	{
		m_color = value;
	}

	int CoreDrawnObject2D_Imp::GetDrawingPriority() const
	{
		return m_drawingPriority;
	}

	void CoreDrawnObject2D_Imp::SetDrawingPriority(int value)
	{
		m_drawingPriority = value;
	}

	Color CoreDrawnObject2D_Imp::GetAbsoluteColor() const
	{
		if (m_parentInfo != nullptr)
		{
			return m_parentInfo->GetInheritedColor() * m_color;
		}
		return m_color;
	}

	int CoreDrawnObject2D_Imp::GetAbsoluteDrawingPriority() const
	{
		if (m_parentInfo != nullptr)
		{
			return m_parentInfo->GetInheritedDrawingPriority() + m_drawingPriority;
		}
		return m_drawingPriority;
	}
}