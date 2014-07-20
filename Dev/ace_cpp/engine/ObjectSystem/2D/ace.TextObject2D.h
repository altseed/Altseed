#pragma once

#include "ace.Object2D.h"

namespace ace
{
	class TextObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreTextObject2D> m_coreObject;
		std::shared_ptr<Font> m_font;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<TextObject2D> Ptr;

		TextObject2D();
		virtual ~TextObject2D();

		/**
		@brief	このオブジェクトを描画する際の描画方向を取得する。
		*/
		const WritingDirection GetWritingDirection() const;

		/**
		@brief	このオブジェクトを描画する際のフォントを取得する。
		*/
		const std::shared_ptr<Font> GetFont() const;

		/**
		@brief 描画するテキストを取得する。
		*/
		const const achar* GetText() const;

		/**
		@brief	このオブジェクトの原点位置を取得する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		const Vector2DF GetCenterPosition() const;

		/**
		@brief	このオブジェクトを描画する際に文字画像に合成する色を取得する。
		*/
		const Color GetColor() const;

		/**
		@brief	このオブジェクトを描画する際に左右反転するかどうかの真偽値を取得する。
		*/
		const bool GetTurnLR() const;

		/**
		@brief	このオブジェクトを描画する際に上下反転するかどうかの真偽値を取得する。
		*/
		const bool GetTurnUL() const;

		/**
		@brief	このオブジェクトの描画優先度を取得します。
		*/
		const int GetDrawingPriority() const;

		/**
		@brief	このオブジェクトを描画する際のブレンドモードを取得します。
		*/
		const AlphaBlend GetAlphaBlendMode() const;

		/**
		@brief	このオブジェクトを描画する際の描画方向を設定します。
		*/
		void SetWritingDirection(WritingDirection writingDirection);

		/**
		@brief	このオブジェクトを描画する際のフォントを設定する。
		*/
		void SetFont(std::shared_ptr<Font> font);

		/**
		@brief 描画するテキストを設定する。
		*/
		void SetText(const achar* text);

		/**
		@brief	このオブジェクトの原点位置を設定する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		void SetCenterPosition(Vector2DF position);

		/**
		@brief	このオブジェクトを描画する際にテクスチャに合成する色を設定する。
		*/
		void SetColor(Color color);

		/**
		@brief	このオブジェクトを描画する際に左右反転するかどうかの真偽値を設定する。
		*/
		void SetTurnLR(bool turnLR);

		/**
		@brief	このオブジェクトを描画する際に上下反転するかどうかの真偽値を設定する。
		*/
		void SetTurnUL(bool turnUL);

		/**
		@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int priority);

		/**
		@brief	このオブジェクトを描画する際のブレンドモードを設定する。
		*/
		void SetAlphaBlendMode(AlphaBlend alphaBlend);

	};
}