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

		int32_t	m_drawingPriority;

		RenderTexture2D_Imp* m_renderTarget;

		bool	isOffscreenMode = false;
		TextureFilterType	textureFilterType = TextureFilterType::Nearest;

		void ResetBuffer();

	public:
		CoreCameraObject2D_Imp(Graphics_Imp* graphics);
		virtual ~CoreCameraObject2D_Imp();

		Object2DType GetObjectType() const override { return Object2DType::Camera; }

		int GetDrawingPriority() const;
		void SetDrawingPriority(int value);

		RectI GetSrc() const;
		void SetSrc(RectI value);

		RectI GetDst() const;
		void SetDst(RectI value);

		Texture2D* GetTexture() override;

		bool GetIsOffscreenMode() override;

		void SetIsOffscreenMode(bool value) override;

		void SetTextureFilterType(TextureFilterType textureFilterType) override;

		TextureFilterType GetTextureFilterType() const override;

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