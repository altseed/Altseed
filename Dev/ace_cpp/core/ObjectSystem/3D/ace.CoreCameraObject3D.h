
#pragma once

#include "ace.CoreObject3D.h"

namespace ace
{
	class CoreCameraObject3D
		: public CoreObject3D
	{
	protected:
		CoreCameraObject3D(){}
		virtual ~CoreCameraObject3D(){}

	public:

		virtual Vector2DI GetWindowSize() const = 0;
		virtual void SetWindowSize(const Vector2DI& size) = 0;

		virtual Vector3DF GetFocus() = 0;
		virtual void SetFocus(const Vector3DF& focus) = 0;

		virtual float GetFieldOfView() = 0;
		virtual void SetFieldOfView(float fov) = 0;

		virtual float GetZFar() = 0;
		virtual void SetZFar(float zfar) = 0;

		virtual float GetZNear() = 0;
		virtual void SetZNear(float znear) = 0;

		virtual void BeginPostEffect(CorePostEffect* postEffect) = 0;
		virtual void EndPostEffect(CorePostEffect* postEffect) = 0;
		
		virtual void SetPostEffectCount(int count) = 0;
		virtual RenderTexture2D* GetDstForPostEffect(int count) = 0;
		virtual RenderTexture2D* GetSrcForPostEffect(int count) = 0;

	};
}
