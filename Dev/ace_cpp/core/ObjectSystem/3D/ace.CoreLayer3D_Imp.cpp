
#include "ace.CoreLayer3D_Imp.h"

namespace ace
{
	CoreLayer3D_Imp::CoreLayer3D_Imp(Graphics* graphics, Log* log, Vector2DI windowSize)
		: m_drawingPriority(0)
	{
	}

	CoreLayer3D_Imp::~CoreLayer3D_Imp()
	{
	}

	void CoreLayer3D_Imp::Draw()
	{

	}

	int32_t CoreLayer3D_Imp::GetDrawingPriority() const
	{
		return m_drawingPriority;
	}

	void CoreLayer3D_Imp::SetDrawingPriority(int value)
	{
		m_drawingPriority = value;
	}
};