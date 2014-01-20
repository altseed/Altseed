#pragma once
#include "ace.CoreObject2D.h"
#include "../common/Math/ace.RectI.h"
#include "../common/Math/ace.Matrix33.h"
#include "../../Graphics/Common/Resource/ace.RenderTexture.h"

namespace ace
{
	class CoreCameraObject2D : public CoreObject2D
	{
	public:
		virtual RectI GetSrc() const = 0;
		virtual void SetSrc(RectI value) = 0;

		virtual RectI GetDst() const = 0;
		virtual void SetDst(RectI value) = 0;

		virtual void SetForRenderTarget() = 0;

#if !SWIG
		virtual void DrawBuffer() = 0;

		virtual Matrix33 GetCameraMatrix() = 0;
#endif
	};
}