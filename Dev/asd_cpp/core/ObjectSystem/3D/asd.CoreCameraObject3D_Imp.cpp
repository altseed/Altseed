
#include "asd.CoreCameraObject3D_Imp.h"
#include "asd.CoreLayer3D_Imp.h"

#include "../PostEffect/asd.CorePostEffect_Imp.h"

#include "../../Graphics/3D/Object/asd.RenderedCameraObject3D.h"

namespace asd
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

	Vector2DI CoreCameraObject3D_Imp::GetWindowSize() const
	{
		return m_object->GetWindowSize();
	}

	void CoreCameraObject3D_Imp::SetWindowSize(const Vector2DI& size)
	{
		m_object->SetWindowSize(size);
	}

	Vector3DF CoreCameraObject3D_Imp::GetFocus()
	{
		return m_object->GetFocus();
	}

	void CoreCameraObject3D_Imp::SetFocus(const Vector3DF& focus)
	{
		m_object->SetFocus(focus);
	}

	float CoreCameraObject3D_Imp::GetFieldOfView()
	{
		return m_object->GetFieldOfView();
	}

	void CoreCameraObject3D_Imp::SetFieldOfView(float fov)
	{
		m_object->SetFieldOfView(fov);
	}

	float CoreCameraObject3D_Imp::GetZFar()
	{
		return m_object->GetZFar();
	}

	void CoreCameraObject3D_Imp::SetZFar(float zfar)
	{
		m_object->SetZFar(zfar);
	}

	float CoreCameraObject3D_Imp::GetZNear()
	{
		return m_object->GetZNear();
	}

	void CoreCameraObject3D_Imp::SetZNear(float znear)
	{
		m_object->SetZNear(znear);
	}

	Matrix44 CoreCameraObject3D_Imp::GetProjectionMatrixDirectly()
	{
		return m_object->GetProjectionMatrixDirectly();
	}

	void CoreCameraObject3D_Imp::SetProjectionMatrixDirectly(Matrix44 projectionMatrix)
	{
		m_object->SetProjectionMatrixDirectly(projectionMatrix);
	}

	bool CoreCameraObject3D_Imp::GetHDRMode() const
	{
		return m_object->GetHDRMode();
	}

	void CoreCameraObject3D_Imp::SetHDRMode(bool value)
	{
		m_object->SetHDRMode(value);
	}

	void CoreCameraObject3D_Imp::BeginPostEffect(CorePostEffect* postEffect)
	{

	}

	void CoreCameraObject3D_Imp::EndPostEffect(CorePostEffect* postEffect)
	{
		auto pe = (CorePostEffect_Imp*) postEffect;

		for (auto& c : pe->GetCommands())
		{
			m_object->AddPostEffectCommand(c);
		}

		pe->ClearCommands();
	}

	void CoreCameraObject3D_Imp::StartAddingPostEffect(int count)
	{
		m_object->StartAddingPostEffect(count);
	}

	RenderTexture2D* CoreCameraObject3D_Imp::GetDstForPostEffect(int count)
	{
		return m_object->GetDstForPostEffect(count);
	}

	RenderTexture2D* CoreCameraObject3D_Imp::GetSrcForPostEffect(int count)
	{
		return m_object->GetSrcForPostEffect(count);
	}

}
