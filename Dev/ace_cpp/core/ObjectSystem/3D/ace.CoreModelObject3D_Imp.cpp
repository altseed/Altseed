
#include "ace.CoreModelObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

#include "../../Graphics/Common/3D/ace.RenderedModelObject3D.h"

namespace ace
{
	CoreModelObject3D_Imp::CoreModelObject3D_Imp(Graphics* graphics)
		: m_object(nullptr)
	{
		m_object = new RenderedModelObject3D(graphics);
		m_commonObject = m_object;
	}

	CoreModelObject3D_Imp::~CoreModelObject3D_Imp()
	{
		SafeRelease(m_object);
	}

	void CoreModelObject3D_Imp::AddMesh(Mesh* mesh)
	{
		m_object->AddMesh(mesh);
	}
}
