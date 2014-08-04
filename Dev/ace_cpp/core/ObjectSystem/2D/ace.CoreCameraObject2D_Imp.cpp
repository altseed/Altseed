#include <algorithm>
#include "ace.CoreCameraObject2D_Imp.h"
#include "../../Graphics/ace.Graphics_Imp.h"

using namespace std;

namespace ace
{
	CoreCameraObject2D_Imp::CoreCameraObject2D_Imp(Graphics_Imp* graphics)
		: m_src(RectI(0, 0, 100, 100))
		, m_dst(RectI(0, 0, 100, 100))
		, m_renderTarget(nullptr)
		, CoreObject2D_Imp(graphics)
	{
		ResetBuffer();
	}

	CoreCameraObject2D_Imp::~CoreCameraObject2D_Imp()
	{
		SafeRelease(m_renderTarget);
	}

	void CoreCameraObject2D_Imp::ResetBuffer()
	{
		SafeRelease(m_renderTarget);

		m_renderTarget = m_graphics->CreateRenderTexture2D_Imp(
			m_src.Width,
			m_src.Height,
			eTextureFormat::TEXTURE_FORMAT_R8G8B8A8_UNORM);
	}


	RectI CoreCameraObject2D_Imp::GetSrc() const
	{
		return m_src;
	}

	void CoreCameraObject2D_Imp::SetSrc(RectI value)
	{
		if (value.Width != m_src.Width || value.Height != m_src.Height)
		{
			m_src = value;
			ResetBuffer();
		}
		else
		{
			m_src = value;
		}
	}

	RectI CoreCameraObject2D_Imp::GetDst() const
	{
		return m_dst;
	}

	void CoreCameraObject2D_Imp::SetDst(RectI value)
	{
		m_dst = value;
	}

	Matrix33 CoreCameraObject2D_Imp::GetCameraMatrix()
	{
		auto translate = Matrix33().SetTranslation(-m_src.X, -m_src.Y);
		return translate;
	}

	void CoreCameraObject2D_Imp::SetForRenderTarget()
	{
		m_graphics->SetRenderTarget(m_renderTarget, nullptr);
		m_graphics->Clear(true, false, Color(0, 0, 0, 0));
	}

	void CoreCameraObject2D_Imp::Draw(Renderer2D* renderer)
	{
		throw "CameraObject2D の Draw 関数は無効です。";
	}

	void CoreCameraObject2D_Imp::FlushToBuffer(Renderer2D* renderer)
	{
		renderer->SetArea(RectF(m_src.X, m_src.Y, m_src.Width, m_src.Height));
		renderer->DrawCache();
	}

	void CoreCameraObject2D_Imp::DrawBuffer(Renderer2D* renderer)
	{
		array<Color, 4> colors;
		fill(begin(colors), end(colors), Color());

		array<Vector2DF, 4> uvs =
		{
			Vector2DF(0, 0),
			Vector2DF(1, 0),
			Vector2DF(1, 1),
			Vector2DF(0, 1),
		};

		array<Vector2DF, 4> vs =
		{
			Vector2DF(m_dst.X, m_dst.Y),
			Vector2DF(m_dst.X + m_dst.Width, m_dst.Y),
			Vector2DF(m_dst.X + m_dst.Width, m_dst.Y + m_dst.Height),
			Vector2DF(m_dst.X, m_dst.Y + m_dst.Height),
		};

		renderer->AddSprite(
			vs.data(),
			colors.data(),
			uvs.data(),
			m_renderTarget,
			AlphaBlend::Blend,
			0);
	}
}