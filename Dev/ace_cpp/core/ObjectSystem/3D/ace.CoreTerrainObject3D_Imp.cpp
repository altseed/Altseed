
#include "ace.CoreTerrainObject3D_Imp.h"

#include "../../Graphics/3D/Object/ace.RenderedTerrainObject3D.h"

namespace ace
{
	CoreTerrainObject3D_Imp::CoreTerrainObject3D_Imp(Graphics* graphics)
		: m_object(nullptr)
	{
		m_object = new RenderedTerrainObject3D(graphics);
		m_commonObject = m_object;
	}

	CoreTerrainObject3D_Imp::~CoreTerrainObject3D_Imp()
	{
		SafeRelease(m_object);
	}

	void CoreTerrainObject3D_Imp::SetTerrain(Terrain3D* terrain)
	{
		m_object->SetTerrain(terrain);
	}
}