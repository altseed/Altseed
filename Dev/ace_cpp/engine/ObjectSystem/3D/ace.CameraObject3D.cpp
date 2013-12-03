
#include "ace.CameraObject3D.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	CameraObject3D::CameraObject3D()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateCameraObject3D());
		m_commonObject = m_coreObject.get();
	}

	CameraObject3D::~CameraObject3D()
	{

	}

	Vector2DI CameraObject3D::GetWindowSize() const
	{
		return m_coreObject->GetWindowSize();
	}

	void CameraObject3D::SetWindowSize(const Vector2DI& size)
	{
		m_coreObject->SetWindowSize(size);
	}

	Vector3DF CameraObject3D::GetFocus()
	{
		return m_coreObject->GetFocus();
	}

	void CameraObject3D::SetFocus(const Vector3DF& focus)
	{
		m_coreObject->SetFocus(focus);
	}

	float CameraObject3D::GetFieldOfView()
	{
		return m_coreObject->GetFieldOfView();
	}

	void CameraObject3D::SetFieldOfView(float fov)
	{
		m_coreObject->SetFieldOfView(fov);
	}

	float CameraObject3D::GetZFar()
	{
		return m_coreObject->GetZFar();
	}

	void CameraObject3D::SetZFar(float zfar)
	{
		m_coreObject->SetZFar(zfar);
	}

	float CameraObject3D::GetZNear()
	{
		m_coreObject->GetZNear();
	}

	void CameraObject3D::SetZNear(float znear)
	{
		m_coreObject->SetZNear(znear);
	}
}