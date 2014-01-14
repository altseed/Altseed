#pragma once
#include "ace.CoreCameraObject2D.h"

namespace ace
{
	class CoreCameraObject2D_Imp : public CoreCameraObject2D
	{
	private:
		RectI m_src;
		RectI m_dst;
		RenderTexture_Imp* m_renderTarget;
		DepthBuffer_Imp* m_depthBuffer;

	public:
		CoreCameraObject2D_Imp();
		virtual ~CoreCameraObject2D_Imp()
		{
		}

		RectI GetSrc() const;
		void SetSrc(RectI value);

		RectI GetDst() const;
		void SetDst(RectI value);

#if !SWIG
		RenderTexture_Imp* GetRenderTarget();
		DepthBuffer_Imp* GetDepthBuffer();

		Matrix33 GetCameraMatrix();
#endif
	};
}