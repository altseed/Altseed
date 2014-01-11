
#include "ace.CameraObject3D.h"
#include "../PostEffect/ace.PostEffect.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	void CameraObject3D::OnUpdateInternal()
	{
		m_coreObject->SetPostEffectCount(m_postEffects.size());

		int32_t count = 0;
		for (auto& p : m_postEffects)
		{
			m_coreObject->BeginPostEffect(p->GetCoreObject());

			m_coreObject->GetSrcForPostEffect(count)->AddRef();
			m_coreObject->GetDstForPostEffect(count)->AddRef();

			auto src = CreateSharedPtrWithReleaseDLL(m_coreObject->GetSrcForPostEffect(count));
			auto dst = CreateSharedPtrWithReleaseDLL(m_coreObject->GetDstForPostEffect(count));

			p->OnDraw(dst, src);

			m_coreObject->EndPostEffect(p->GetCoreObject());

			count++;
		}
	}

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
		return m_coreObject->GetZNear();
	}

	void CameraObject3D::SetZNear(float znear)
	{
		m_coreObject->SetZNear(znear);
	}

	void CameraObject3D::AddPostEffect(const std::shared_ptr<PostEffect>& postEffect)
	{
		m_postEffects.push_back(postEffect);
	}

	void CameraObject3D::ClearPostEffects()
	{
		m_postEffects.clear();
	}
}