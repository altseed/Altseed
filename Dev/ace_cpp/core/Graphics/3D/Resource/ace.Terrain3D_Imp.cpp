
#include "ace.Terrain3D_Imp.h"
#include "../../ace.Graphics_Imp.h"

namespace ace
{
	Terrain3D_Imp::Terrain3D_Imp(Graphics* graphics)
		: m_graphics(graphics)
	{
		SafeAddRef(graphics);
	}

	Terrain3D_Imp::~Terrain3D_Imp()
	{
		SafeRelease(m_graphics);
	}

}