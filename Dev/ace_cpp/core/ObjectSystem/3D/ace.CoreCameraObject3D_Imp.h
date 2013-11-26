
#pragma once

#include "ace.CoreCameraObject3D.h"
#include "ace.CoreObject3D_Imp.h"

namespace ace
{
	class CoreCameraObject3D_Imp
		: public CoreCameraObject3D
		, public CoreObject3D_Imp
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
	};
}
