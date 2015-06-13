
#pragma once

#include "asd.CoreCameraObject3D.h"
#include "asd.CoreObject3D_Imp.h"

#include <Math/asd.Vector3DF.h>

namespace asd
{
	class CoreCameraObject3D_Imp
		: public CoreCameraObject3D
		, public CoreObject3D_Imp
		, public ReferenceObject
	{
	private:
		RenderedCameraObject3D*	m_object;

	public:
		CoreCameraObject3D_Imp(Graphics* graphics);

	private:
		virtual ~CoreCameraObject3D_Imp();
	public:

		Vector2DI GetWindowSize() const;
		void SetWindowSize(const Vector2DI& size);

		Vector3DF GetFocus();
		void SetFocus(const Vector3DF& focus);

		float GetFieldOfView();
		void SetFieldOfView(float fov);

		float GetZFar();
		void SetZFar(float zfar);

		float GetZNear();
		void SetZNear(float znear);

		bool GetHDRMode() const override;
		void SetHDRMode(bool value) override;

		void BeginPostEffect(CorePostEffect* postEffect) override;
		void EndPostEffect(CorePostEffect* postEffect) override;

		void StartAddingPostEffect(int count) override;
		RenderTexture2D* GetDstForPostEffect(int count) override;
		RenderTexture2D* GetSrcForPostEffect(int count) override;

		RenderedObject3DType GetObjectType() { return RenderedObject3DType::Camera; }


// 共通部分
		virtual bool GetIsDrawn() const { return CoreObject3D_Imp::GetIsDrawn(); }
		virtual void SetIsDrawn(bool value) { return CoreObject3D_Imp::SetIsDrawn(value); }
		virtual Vector3DF GetPosition() const { return CoreObject3D_Imp::GetPosition(); }
		virtual void SetPosition(Vector3DF position) { CoreObject3D_Imp::SetPosition(position); }

		virtual Vector3DF GetRotation() const{ return CoreObject3D_Imp::GetRotation(); }
		virtual void SetRotation(const Vector3DF& rot){ CoreObject3D_Imp::SetRotation(rot); }

		virtual Vector3DF GetScale() const{ return CoreObject3D_Imp::GetScale(); }
		virtual void SetScale(const Vector3DF& scale){ CoreObject3D_Imp::SetScale(scale); }

		virtual void SetLayer(CoreLayer3D* layer) { CoreObject3D_Imp::SetLayer(layer); }

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif

	};
}
