
#include "ace.RenderedCameraObject3D.h"
#include "../ace.Graphics_Imp.h"

#include "../Resource/ace.RenderTexture_Imp.h"
#include "../Resource/ace.DepthBuffer_Imp.h"

#include "../2D/ace.PostEffectRenderer.h"

namespace ace
{
	RenderedCameraObject3D::RenderedCameraObject3D(Graphics* graphics)
		: RenderedObject3D(graphics)
		, m_depthBuffer_FR(nullptr)
		, m_postEffectRenderer(nullptr)
	{
		m_renderTarget_FR[0] = nullptr;
		m_renderTarget_FR[1] = nullptr;

		m_values.size = Vector2DI();
		m_values.fov = 0.0f;
		m_values.zfar = 0.0f;
		m_values.znear = 0.0f;

		m_values.postEffectCount = 0;
		m_values_FR.postEffectCount = 0;

		m_postEffectRenderer = PostEffectRenderer::Create(graphics);
	}

	RenderedCameraObject3D::~RenderedCameraObject3D()
	{
		SafeRelease(m_renderTarget_FR[0]);
		SafeRelease(m_renderTarget_FR[1]);
		SafeRelease(m_depthBuffer_FR);
		SafeRelease(m_postEffectRenderer);
	}

	void RenderedCameraObject3D::Flip()
	{
		RenderedObject3D::Flip();

		if (m_values.size != m_values_FR.size)
		{
			m_values_FR.size = m_values.size;
			SafeRelease(m_renderTarget_FR[0]);
			SafeRelease(m_renderTarget_FR[1]);
			SafeRelease(m_depthBuffer_FR);

			m_renderTarget_FR[0] = GetGraphics()->CreateRenderTexture_Imp(m_values_FR.size.X, m_values_FR.size.Y, TEXTURE_FORMAT_RGBA8888);
			m_renderTarget_FR[1] = GetGraphics()->CreateRenderTexture_Imp(m_values_FR.size.X, m_values_FR.size.Y, TEXTURE_FORMAT_RGBA8888);
			m_depthBuffer_FR = GetGraphics()->CreateDepthBuffer_Imp(m_values_FR.size.X, m_values_FR.size.Y);
		}

		m_values_FR.size = m_values.size;
		m_values_FR.znear = m_values.znear;
		m_values_FR.zfar = m_values.zfar;
		m_values_FR.fov = m_values.fov;
		m_values_FR.focus = m_values.focus;
		m_values_FR.postEffectCount = m_values.postEffectCount;

		m_postEffectCommands_FR.clear();
		m_postEffectCommands_FR.resize(m_postEffectCommands.size());
		std::copy(m_postEffectCommands.begin(), m_postEffectCommands.end(), m_postEffectCommands_FR.begin());
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

	void RenderedCameraObject3D::SetPostEffectCount(int32_t postEffectCount)
	{
		m_values.postEffectCount = postEffectCount;
	}

	void RenderedCameraObject3D::AddPostEffectCommand(std::shared_ptr<Material2DCommand> command)
	{
		m_postEffectCommands.push_back(command);
	}

	RenderTexture2D* RenderedCameraObject3D::GetDstForPostEffect(int32_t count)
	{
		if (m_values.postEffectCount % 2 == 0)
		{
			if (count % 2 == 1)
			{
				return m_renderTarget_FR[0];
			}
			else
			{
				return m_renderTarget_FR[1];
			}
		}
		else
		{
			if (count % 2 == 1)
			{
				return m_renderTarget_FR[1];
			}
			else
			{
				return m_renderTarget_FR[0];
			}
		}

		return nullptr;
	}

	RenderTexture2D* RenderedCameraObject3D::GetSrcForPostEffect(int32_t count)
	{
		if (m_values.postEffectCount % 2 == 0)
		{
			if (count % 2 == 1)
			{
				return m_renderTarget_FR[1];
			}
			else
			{
				return m_renderTarget_FR[0];
			}
		}
		else
		{
			if (count % 2 == 1)
			{
				return m_renderTarget_FR[0];
			}
			else
			{
				return m_renderTarget_FR[1];
			}
		}

		return nullptr;
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
			Vector3DF(mat.Values[0][3], mat.Values[1][3], mat.Values[2][3]),
			m_values_FR.focus,
			Vector3DF(0, 1, 0));
	}

	void RenderedCameraObject3D::ApplyPostEffects_FR()
	{
		for (auto& c : m_postEffectCommands_FR)
		{
			m_postEffectRenderer->DrawOnTexture2DWithMaterialWithCommand(c);
		}
	}

	RenderTexture_Imp* RenderedCameraObject3D::GetRenderTarget_FR()
	{ 
		return m_renderTarget_FR[m_values_FR.postEffectCount % 2]; 
	}

	RenderTexture_Imp* RenderedCameraObject3D::GetAffectedRenderTarget_FR()
	{ 
		return m_renderTarget_FR[0];
	}
}