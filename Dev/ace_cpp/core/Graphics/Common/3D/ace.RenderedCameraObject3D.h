
#pragma once

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector3DF.h>

#include "ace.RenderedObject3D.h"

namespace ace
{
	class RenderedCameraObject3D
		: public RenderedObject3D
	{
	private:

		struct
		{
			float	zfar;
			float	znear;
			float	fov;

			Vector2DI	size;
			Vector3DF	focus;
		} m_values;

		struct
		{
			float	zfar;
			float	znear;
			float	fov;
			Vector2DI	size;
			Vector3DF	focus;
			Matrix44	cameraMatrix;
			Matrix44	projectionMatrix;

		} m_values_FR;

		RenderTexture_Imp*	m_renderTarget_FR;
		DepthBuffer_Imp*	m_depthBuffer_FR;

	public:
		RenderedCameraObject3D(Graphics* graphics);
		virtual ~RenderedCameraObject3D();

		void Flip() override;
		void Rendering(RenderingProperty& prop) override;

		Vector2DI GetWindowSize() const { return m_values.size; }
		void SetWindowSize( const Vector2DI& size );

		Vector3DF GetFocus(){ return m_values.focus; }
		void SetFocus(const Vector3DF& focus);

		float GetFieldOfView(){ return m_values.fov; }
		void SetFieldOfView(float fov);

		float GetZFar() { return m_values.zfar; }
		void SetZFar(float zfar);

		float GetZNear(){ return m_values.znear; }
		void SetZNear(float znear);

		void CalculateMatrix_FR() override;

		RenderTexture_Imp* GetRenderTarget_FR() { return m_renderTarget_FR; }
		DepthBuffer_Imp* GetDepthBuffer_FR() { return m_depthBuffer_FR; }

		const Matrix44& GetCameraMatrix_FR() { return m_values_FR.cameraMatrix; }
		const Matrix44& GetProjectionMatrix_FR() { return m_values_FR.projectionMatrix; }

		eRenderedObject3DType GetObjectType() const override { return RENDERED_OBJECT3D_TYPE_CAMERA; }
	};
}