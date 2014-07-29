
#include "ace.CoreDirectionalLightObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

#include "../../Graphics/3D/Object/ace.RenderedDirectionalLightObject3D.h"

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

	Color CoreDirectionalLightObject3D_Imp::GetColor()
	{
		return m_object->GetColor();
	}

	void CoreDirectionalLightObject3D_Imp::SetColor(const Color& color)
	{
		m_object->SetColor(color);
	}
}
