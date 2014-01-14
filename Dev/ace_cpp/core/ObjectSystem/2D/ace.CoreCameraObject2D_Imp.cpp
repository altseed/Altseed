#include "ace.CoreCameraObject2D_Imp.h"

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


	RenderTexture_Imp* CoreCameraObject2D_Imp::GetRenderTarget()
	{
		return m_renderTarget;
	}

	Matrix33 CoreCameraObject2D_Imp::GetCameraMatrix()
	{
		auto matrix = Matrix33();
		matrix.SetTranslation(-m_src.X, -m_src.Y);
		matrix.SetScaling(m_dst.Width/m_src.Width, m_dst.Height/m_src.Height);
		return matrix;
	}
}