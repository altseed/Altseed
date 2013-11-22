
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

	Vector3DF CoreMeshObject3D_Imp::GetPosition() const
	{
		return m_object->GetPosition();
	}

	void CoreMeshObject3D_Imp::SetPosition(Vector3DF position)
	{
		m_object->SetPosition(position);
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
