
#pragma once

#include "ace.CoreCameraObject3D.h"
#include "ace.CoreObject3D_Imp.h"

#include <Math/ace.Vector3DF.h>

namespace ace
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

		eRenderedObject3DType GetObjectType() { return eRenderedObject3DType::RENDERED_OBJECT3D_TYPE_CAMERA; }



		virtual Vector3DF GetPosition() const { return CoreObject3D_Imp::GetPosition(); }
		virtual void SetPosition(Vector3DF position) { CoreObject3D_Imp::SetPosition(position); }
		virtual void SetLayer(CoreLayer3D* layer) { CoreObject3D_Imp::SetLayer(layer); }

#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}
