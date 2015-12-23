
#pragma once
#include "../common/Math/asd.Vector2DF.h"
#include "../common/Graphics/asd.Color.h"
#include "../../Graphics/Resource/asd.Font.h"
#include "asd.CoreObject2D.h"
#include "asd.DrawnObject2D.h"

namespace asd
{

	class CoreTextObject2D
		: public DrawnObject2D
	{
	public:
		CoreTextObject2D() {}
		virtual ~CoreTextObject2D() {}

		/**
		@brief	このオブジェクトを描画する際の描画方向を取得する。
		*/
		virtual WritingDirection GetWritingDirection() const = 0;

		/**
		@brief	このオブジェクトを描画する際のフォントを取得する。
		*/
		virtual Font* GetFont() const = 0;

		/**
		@brief 描画するテキストを取得する。
		*/
		virtual const achar* GetText() const = 0;

		/**
		@brief	このオブジェクトの原点位置を取得する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		virtual Vector2DF GetCenterPosition() const = 0;

		/**
		@brief	このオブジェクトを描画する際に文字画像に合成する色を取得する。
		*/
		virtual Color GetColor() const = 0;

		/**
		@brief	このオブジェクトを描画する際に左右反転するかどうかの真偽値を取得する。
		*/
		virtual bool GetTurnLR() const = 0;

		/**
		@brief	このオブジェクトを描画する際に上下反転するかどうかの真偽値を取得する。
		*/
		virtual bool GetTurnUL() const = 0;

		/**
		@brief	このオブジェクトの描画優先度を取得します。
		*/
		virtual int GetDrawingPriority() const = 0;

		/**
		@brief	このオブジェクトを描画する際のブレンドモードを取得します。
		*/
		virtual AlphaBlendMode GetAlphaBlendMode() const = 0;

		/**
		@brief	このオブジェクトを描画する際の描画方向を設定します。
		*/
		virtual void SetWritingDirection(WritingDirection writingDirection) = 0;

		/**
		@brief	このオブジェクトを描画する際のフォントを設定する。
		*/
		virtual void SetFont(Font* font) = 0;

		/**
		@brief 描画するテキストを設定する。
		*/
		virtual void SetText(const achar* text) = 0;

		/**
		@brief	このオブジェクトの原点位置を設定する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		virtual void SetCenterPosition(Vector2DF position) = 0;

		/**
		@brief	このオブジェクトを描画する際にテクスチャに合成する色を設定する。
		*/
		virtual void SetColor(Color color) = 0;

		/**
		@brief	このオブジェクトを描画する際に左右反転するかどうかの真偽値を設定する。
		*/
		virtual void SetTurnLR(bool turnLR) = 0;

		/**
		@brief	このオブジェクトを描画する際に上下反転するかどうかの真偽値を設定する。
		*/
		virtual void SetTurnUL(bool turnUL) = 0;

		/**
		@brief	このオブジェクトの描画優先度を設定する。
		*/
		virtual void SetDrawingPriority(int priority) = 0;

		/**
		@brief	このオブジェクトを描画する際のブレンドモードを設定する。
		*/
		virtual void SetAlphaBlendMode(AlphaBlendMode alphaBlend) = 0;

		/**
		@brief　このオブジェクトを描画する際のテクスチャフィルタの種類を設定する。
		*/
		virtual void SetTextureFilterType(TextureFilterType textureFilterType) = 0;

		/**
		@brief　このオブジェクトを描画する際のテクスチャフィルタの種類を取得する。
		*/
		virtual TextureFilterType GetTextureFilterType() const = 0;

		/**
		@brief テキストを描画する際の文字間値を設定する。
		*/
		virtual void SetLetterSpacing(float letterSpacing) = 0;

		/**
		@brief テキストを描画する際の文字間値を取得する。
		*/
		virtual float GetLetterSpacing() const = 0;

		/**
		@brief テキストを描画する際の行間値を設定する。
		*/
		virtual void SetLineSpacing(float lineSpacing) = 0;

		/**
		@brief テキストを描画する際の行間値を取得する。
		*/
		virtual float GetLineSpacing() const = 0;
	};
}