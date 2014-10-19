
#include "ace.CoreMassModelObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

#include "../../Graphics/3D/Object/ace.RenderedMassModelObject3D.h"

namespace ace
{
	CoreMassModelObject3D_Imp::CoreMassModelObject3D_Imp(Graphics* graphics)
		: m_object(nullptr)
	{
		m_object = new RenderedMassModelObject3D(graphics);
		m_commonObject = m_object;
	}

	CoreMassModelObject3D_Imp::~CoreMassModelObject3D_Imp()
	{
		SafeRelease(m_object);
	}

	void CoreMassModelObject3D_Imp::SetModel(MassModel* model)
	{
		m_object->SetModel(model);
	}
}
