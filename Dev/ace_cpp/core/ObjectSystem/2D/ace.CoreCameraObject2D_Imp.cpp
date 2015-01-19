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
			TextureFormat::R8G8B8A8_UNORM);
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
		auto pos = GetPosition();

		auto parentMatrix = m_transform.GetParentsMatrix();
		auto matrix = m_transform.GetMatrixToTransform();
		auto mat = parentMatrix * matrix;

		Vector2DF t;
		t.X = mat.Values[0][2];
		t.Y = mat.Values[1][2];

		float sc[2];
		for (int m = 0; m < 2; m++)
		{
			sc[m] = sqrt(mat.Values[0][m] * mat.Values[0][m] + mat.Values[1][m] * mat.Values[1][m]);
		}

		Vector2DF s;
		s.X = sc[0];
		s.Y = sc[1];
		
		auto r = mat;
		for (int m = 0; m < 2; m++)
		{
			for (int n = 0; n < 2; n++)
			{
				r.Values[n][m] = mat.Values[n][m] / sc[n];
			}
		}

		r.Values[0][2] = 0.0f;
		r.Values[1][2] = 0.0f;
		
		float angle = -atan2(r.Values[1][0], r.Values[0][0]);

		renderer->SetArea(RectF(m_src.X + t.X, m_src.Y + t.Y, m_src.Width * s.X, m_src.Height * s.Y), angle);
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