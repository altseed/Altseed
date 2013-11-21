
#include "ace.CoreMeshObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

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

	void CoreMeshObject3D_Imp::SetLayer(CoreLayer3D* layer)
	{
		if (layer != nullptr)
		{
			((CoreLayer3D_Imp*) layer)->GetRenderer()->AddObject(m_object);
		}
		else
		{
			((CoreLayer3D_Imp*) layer)->GetRenderer()->RemoveObject(m_object);
		}
	}
}
