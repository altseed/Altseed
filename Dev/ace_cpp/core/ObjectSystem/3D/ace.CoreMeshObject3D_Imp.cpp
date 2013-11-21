
#include "ace.CoreMeshObject3D_Imp.h"
#include "../Core/Graphics/Common/3D/ace.RenderedMeshObject3D.h"

namespace ace
{
	CoreMeshObject3D_Imp::CoreMeshObject3D_Imp(Graphics* graphics)
		: m_object(nullptr)
	{
		m_object = new RenderedMeshObject3D(graphics);
	}

	CoreMeshObject3D_Imp::~CoreMeshObject3D_Imp()
	{
		SafeRelease(m_object);
	}

}
