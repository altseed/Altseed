
#include "ace.CoreMeshObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

#include "../Core/Graphics/Common/3D/ace.RenderedMeshObject3D.h"

namespace ace
{
	CoreMeshObject3D_Imp::CoreMeshObject3D_Imp(Graphics* graphics)
		: m_object(nullptr)
	{
		m_object = new RenderedMeshObject3D(graphics);
		m_commonObject = m_object;
	}

	CoreMeshObject3D_Imp::~CoreMeshObject3D_Imp()
	{
		SafeRelease(m_object);
	}

	void CoreMeshObject3D_Imp::SetMesh(Mesh* mesh)
	{
		m_object->SetMesh(mesh);
	}
}
