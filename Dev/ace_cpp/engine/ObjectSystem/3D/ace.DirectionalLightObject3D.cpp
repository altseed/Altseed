
#include "ace.DirectionalLightObject3D.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	DirectionalLightObject3D::DirectionalLightObject3D()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateDirectionalLightObject3D());
		m_commonObject = m_coreObject.get();
	}

	DirectionalLightObject3D::~DirectionalLightObject3D()
	{

	}

	Color DirectionalLightObject3D::GetColor()
	{
		return m_coreObject->GetColor();
	}

	void DirectionalLightObject3D::SetColor(const Color& color)
	{
		m_coreObject->SetColor(color);
	}
}