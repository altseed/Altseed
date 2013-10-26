
#include "ace.RenderedCameraObject3D.h"
#include "../ace.Graphics_Imp.h"

#include "../Resource/ace.RenderTexture_Imp.h"
#include "../Resource/ace.DepthBuffer_Imp.h"

namespace ace
{
	RenderedCameraObject3D::RenderedCameraObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
		, m_renderTarget_FR(nullptr)
		, m_depthBuffer_FR(nullptr)
	{
		m_values.size = Vector2DI();
		m_values.fov = 0.0f;
		m_values.zfar = 0.0f;
		m_values.znear = 0.0f;
	}

	RenderedCameraObject3D::~RenderedCameraObject3D()
	{
		SafeRelease(m_renderTarget_FR);
		SafeRelease(m_depthBuffer_FR);
	}

	void RenderedCameraObject3D::Flip()
	{
		RenderedObject3D::Flip();

		if (m_values.size != m_values_FR.size)
		{
			m_values_FR.size = m_values.size;
			SafeRelease(m_renderTarget_FR);
			SafeRelease(m_depthBuffer_FR);

			m_renderTarget_FR = GetGraphics()->CreateRenderTexture_Imp(m_values_FR.size.X, m_values_FR.size.Y, TEXTURE_FORMAT_RGBA8888);
			m_depthBuffer_FR = GetGraphics()->CreateDepthBuffer_Imp(m_values_FR.size.X, m_values_FR.size.Y);
		}

		m_values_FR.size = m_values.size;
		m_values_FR.znear = m_values.znear;
		m_values_FR.zfar = m_values.zfar;
		m_values_FR.fov = m_values.fov;
		m_values_FR.focus = m_values.focus;
	}

	void RenderedCameraObject3D::Rendering(RenderingProperty& prop)
	{

	}

	void RenderedCameraObject3D::SetFocus(const Vector3DF& focus)
	{
		m_values.focus = focus;
	}

	void RenderedCameraObject3D::SetWindowSize(const Vector2DI& size)
	{
		m_values.size = size;
	}

	void RenderedCameraObject3D::SetFieldOfView(float fov)
	{
		m_values.fov = fov;
	}

	void RenderedCameraObject3D::SetZFar(float zfar)
	{
		m_values.zfar = zfar;
	}

	void RenderedCameraObject3D::SetZNear(float znear)
	{
		m_values.znear = znear;
	}

	void RenderedCameraObject3D::CalculateMatrix_FR()
	{
		RenderedObject3D::CalculateMatrix_FR();

		auto& mat = GetLocalMatrix_FR();

		m_values_FR.projectionMatrix.PerspectiveFovRH(
			m_values_FR.fov / 180.0f * 3.141592f,
			(float) m_values_FR.size.X / (float) m_values_FR.size.Y,
			m_values_FR.znear,
			m_values_FR.zfar);

		m_values_FR.cameraMatrix.LookAtRH(
			Vector3DF(mat.Values[3][0], mat.Values[3][1], mat.Values[3][2]),
			m_values_FR.focus,
			Vector3DF(0, 1, 0));
	}
}