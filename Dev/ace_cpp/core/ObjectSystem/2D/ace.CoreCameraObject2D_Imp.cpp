#include <algorithm>
#include "ace.CoreCameraObject2D_Imp.h"
#include "../../Graphics/Common/ace.Graphics_Imp.h"

using namespace std;

namespace ace
{
	CoreCameraObject2D_Imp::CoreCameraObject2D_Imp()
		: m_src(RectI(0, 0, 0, 0))
		, m_dst(RectI(0, 0, 0, 0))
		, m_renderTarget(nullptr)
	{
	}

	RectI CoreCameraObject2D_Imp::GetSrc() const
	{
		return m_src;
	}

	void CoreCameraObject2D_Imp::SetSrc(RectI value)
	{
		m_src = value;
	}

	RectI CoreCameraObject2D_Imp::GetDst() const
	{
		return m_dst;
	}

	void CoreCameraObject2D_Imp::SetDst(RectI value)
	{
		m_dst = value;
	}

	void CoreCameraObject2D_Imp::SetForRenderTarget()
	{
		auto graphics = (Graphics_Imp*)(m_objectInfo.GetLayer()->GetGraphicsImp());
		graphics->SetRenderTarget(m_renderTarget, m_depthBuffer);
	}

	Matrix33 CoreCameraObject2D_Imp::GetCameraMatrix()
	{
		auto matrix = Matrix33();
		matrix.SetScaling(m_dst.Width / m_src.Width, m_dst.Height / m_src.Height);
		matrix.SetTranslation(-m_src.X, -m_src.Y);
		return matrix;
	}

	void CoreCameraObject2D_Imp::Draw(Matrix33 cameraMatrix)
	{
		throw "CameraObject2D の Draw 関数は無効です。";
	}

	void CoreCameraObject2D_Imp::DrawBuffer()
	{
		auto renderer = m_objectInfo.GetLayer()->GetRenderer();

		array<Color, 4> colors;
		fill(begin(colors), end(colors), Color());

		array<Vector2DF, 4> uvs =
		{
			Vector2DF(0, 0),
			Vector2DF(1, 0),
			Vector2DF(1, 1),
			Vector2DF(0, 1),
		};

		renderer->AddSprite(
			m_dst.GetVertexes().data(),
			colors.data(),
			uvs.data(),
			m_renderTarget,
			eAlphaBlend::ALPHA_BLEND_BLEND,
			0);
	}
}