
#include "ace.CoreDirectionalLightObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

#include "../Core/Graphics/Common/3D/ace.RenderedDirectionalLightObject3D.h"

namespace ace
{
	CoreDirectionalLightObject3D_Imp::CoreDirectionalLightObject3D_Imp(Graphics* graphics)
		: m_object(nullptr)
	{
		m_object = new RenderedDirectionalLightObject3D(graphics);
		m_commonObject = m_object;
	}

	CoreDirectionalLightObject3D_Imp::~CoreDirectionalLightObject3D_Imp()
	{
		SafeRelease(m_object);
	}
}
