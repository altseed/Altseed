
#include "ace.CoreCameraObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

#include "../Core/Graphics/Common/3D/ace.RenderedCameraObject3D.h"

namespace ace
{
	CoreCameraObject3D_Imp::CoreCameraObject3D_Imp(Graphics* graphics)
		: m_object(nullptr)
	{
		m_object = new RenderedCameraObject3D(graphics);
		m_commonObject = m_object;
	}

	CoreCameraObject3D_Imp::~CoreCameraObject3D_Imp()
	{
		SafeRelease(m_object);
	}
}
