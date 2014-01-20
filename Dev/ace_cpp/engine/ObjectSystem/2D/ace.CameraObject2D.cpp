#include "ace.CameraObject2D.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	CameraObject2D::CameraObject2D()
		: m_coreObject(nullptr)
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateCameraObject2D());
	}

	CameraObject2D::~CameraObject2D()
	{
	}

	CoreObject2D* CameraObject2D::GetCoreObject() const
	{
		return m_coreObject.get();
	}
}