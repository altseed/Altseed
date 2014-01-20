#pragma once
#include <memory>
#include "ace.Object2D.h"

namespace ace
{
	class TextureObject2D : public Object2D
	{
	private:
		std::shared_ptr<CoreTextureObject2D> m_coreObject;
		std::shared_ptr<Texture2D> m_texturePtr;

		CoreObject2D* GetCoreObject() const;

	public:
		/**
			@brief	コンストラクタ
		*/
		TextureObject2D();
		/**
			@brief	デストラクタ
		*/
		virtual ~TextureObject2D();


		/**
			@brief	描画するテクスチャを取得する。
		*/
		std::shared_ptr<Texture2D> GetTexture() const;
		/**
			@brief	描画するテクスチャを設定する。
		*/
		void SetTexture(std::shared_ptr<Texture2D> texture);

		/**
			@brief	描画する際の中心座標を取得する。
		*/
		Vector2DF GetCenterPosition() const;
		/**
			@brief	描画する際の中心座標を設定する。
		*/
		void SetCenterPosition(Vector2DF position);

		/**
			@brief	描画する際に合成する色を取得する。
		*/
		Color GetColor() const;
		/**
			@brief	描画する際に合成する色を設定する。
		*/
		void SetColor(Color color);

		/**
			@brief	左右反転して描画するかどうかを取得する。
		*/
		bool GetTurnLR() const;
		/**
			@brief	左右反転して描画するかどうかを設定する。
		*/
		void SetTurnLR(bool turnLR);

		/**
			@brief	上下反転して描画するかどうかを取得する。
		*/
		bool GetTurnUL() const;
		/**
			@brief	上下反転して描画するかどうかを設定する。
		*/
		void SetTurnUL(bool turnUL);

		/**
			@brief	このオブジェクトの描画優先度を取得する。
		*/
		int GetDrawingPriority() const;
		/**
			@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int priority);

		/**
			@brief	このオブジェクトのブレンディング モードを取得する。
		*/
		eAlphaBlend GetAlphaBlendMode() const;
		/**
			@brief	このオブジェクトのブレンディング モードを設定する。
		*/
		void SetAlphaBlendMode(eAlphaBlend alphaBlend);
	};
}