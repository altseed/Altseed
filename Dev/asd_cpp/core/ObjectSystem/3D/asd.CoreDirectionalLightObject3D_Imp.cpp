
#include "asd.CoreDirectionalLightObject3D_Imp.h"
#include "asd.CoreLayer3D_Imp.h"

#include "../../Graphics/3D/Object/asd.RenderedDirectionalLightObject3D.h"

namespace asd
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

	float CoreDirectionalLightObject3D_Imp::GetIntensity()
	{
		return m_object->GetIntensity();
	}

	void CoreDirectionalLightObject3D_Imp::SetIntensity(float intensity)
	{
		m_object->SetIntensity(intensity);
	}
}
