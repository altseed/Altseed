
#include "ace.CoreLayer_Imp.h"

#include "../Graphics/Common/ace.Graphics_Imp.h"

namespace ace
{

	CoreLayer_Imp::CoreLayer_Imp(Graphics* graphics)
		: m_graphics(nullptr)
		, m_isDrawn(true)
		, m_drawingPriority(0)
	{
		m_graphics = (Graphics_Imp*) graphics;
		SafeAddRef(m_graphics);
	}

	CoreLayer_Imp::~CoreLayer_Imp()
	{
		SafeRelease(m_graphics);
	}

	int CoreLayer_Imp::GetDrawingPriority() const
	{
		return m_drawingPriority;
	}

	void CoreLayer_Imp::SetDrawingPriority(int value)
	{
		m_drawingPriority = value;
	}

	bool CoreLayer_Imp::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void CoreLayer_Imp::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
	}

}