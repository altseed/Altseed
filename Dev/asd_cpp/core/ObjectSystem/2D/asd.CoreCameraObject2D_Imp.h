#pragma once
#include "asd.CoreCameraObject2D.h"
#include "asd.CoreObject2D_Imp.h"

namespace asd
{
	class CoreCameraObject2D_Imp
		: public CoreCameraObject2D
		, public CoreObject2D_Imp
		, public ReferenceObject
	{
	private:
		RectI m_src;
		RectI m_dst;

		RenderTexture2D_Imp* m_renderTarget;

		void ResetBuffer();

	public:
		CoreCameraObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreCameraObject2D_Imp();

		Object2DType GetObjectType() const override { return Object2DType::Camera; }

		RectI GetSrc() const;
		void SetSrc(RectI value);

		RectI GetDst() const;
		void SetDst(RectI value);

#if !SWIG
		Matrix33 GetCameraMatrix();

		void SetForRenderTarget();
		void FlushToBuffer(Renderer2D* renderer);
		void DrawBuffer(Renderer2D* renderer);

		void Draw(Renderer2D* renderer);
#endif
		/*
		CORE_OBJECT2D_IMP_COMMON

		CORE_OBJECT2D_IMP_TRANSFORM

		CORE_OBJECT2D_IMP_CHILD
		*/

#include "asd.CoreObject2D_Imp_Methods.h"
	};
}