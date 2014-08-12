#pragma once
#include "ace.CoreObject2D.h"
#include "../common/Math/ace.RectI.h"
#include "../common/Math/ace.Matrix33.h"
#include "../../Graphics/Resource/ace.RenderTexture2D.h"

namespace ace
{
	class CoreCameraObject2D 
		: public CoreObject2D
	{
	public:
		virtual RectI GetSrc() const = 0;
		virtual void SetSrc(RectI value) = 0;

		virtual RectI GetDst() const = 0;
		virtual void SetDst(RectI value) = 0;

#if !SWIG
		virtual Matrix33 GetCameraMatrix() = 0;

		/**
			@brief	このカメラのバッファを描画対象に指定します。
		*/
		virtual void SetForRenderTarget() = 0;
		
		/**
			@brief	Rendererに登録された内容を、このカメラのバッファに描画します。
		*/
		virtual void FlushToBuffer(Renderer2D* renderer) = 0;

		/**
			@brief	このカメラのバッファの内容に対する描画を、指定したRendererに登録します。
			@param	renderer	登録するRenderer
		*/
		virtual void DrawBuffer(Renderer2D* renderer) = 0;
#endif
	};
}