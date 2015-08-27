#include "asd.CameraObject2D.h"

namespace asd
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

	RectI CameraObject2D::GetSrc() const
	{
		return m_coreObject->GetSrc();
	}

	void CameraObject2D::SetSrc(RectI value)
	{
		m_coreObject->SetSrc(value);
	}

	RectI CameraObject2D::GetDst() const
	{
		return m_coreObject->GetDst();
	}

	void CameraObject2D::SetDst(RectI value)
	{
		m_coreObject->SetDst(value);
	}

	bool CameraObject2D::GetIsOffscreenMode()
	{
		return m_coreObject->GetIsOffscreenMode();
	}

	void CameraObject2D::SetIsOffscreenMode(bool value)
	{
		m_coreObject->SetIsOffscreenMode(value);
	}

	std::shared_ptr<Texture2D> CameraObject2D::GetTexture()
	{
		auto texture = m_coreObject->GetTexture();
		SafeAddRef(texture);
		return CreateSharedPtrWithReleaseDLL(texture);
	}
}