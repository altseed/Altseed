#pragma once

#include "../../ace.CoreToEngine.h"

namespace ace
{
	class MapObject2D;

	class Chip2D
	{
		friend class MapObject2D;
	private:
		std::shared_ptr<Texture2D> m_texture;
		std::shared_ptr<CoreChip2D> m_coreObject;
	protected:
		std::shared_ptr<CoreChip2D> GetCoreObject() const;

	public:

		Chip2D();
		~Chip2D(){}

		/**
		@brief	描画するテクスチャを設定する。
		@param	texture	描画するテクスチャ
		*/
		void SetTexture(std::shared_ptr<Texture2D> texture);


		/**
		@brief	描画するテクスチャを取得する。
		@return	描画するテクスチャ
		*/
		std::shared_ptr<Texture2D> GetTexture();


		/**
		@brief	このチップのテクスチャ上の描画範囲を取得する。
		@return	テクスチャ上の描画範囲
		*/
		RectF GetSrc() const;


		/**
		@brief	このチップのテクスチャ上の描画範囲を設定する。
		@param	src	テクスチャ上の描画範囲
		*/
		void SetSrc(RectF src);

		/**
		@brief このチップのマップオブジェクト2Dに対する相対的な位置を取得する。
		@return 相対的な描画位置
		*/
		Vector2DF GetPosition() const;

		/**
		@brief このチップのマップオブジェクト2Dに対する相対的な位置を設定する。
		@param	position	相対的な描画位置
		*/
		void SetPosition(Vector2DF position);

		/**
		@brief このチップのマップオブジェクト2Dに対する相対的な回転角度を取得する。
		@return 相対的な回転角度
		*/
		float GetAngle() const;

		/**
		@brief　このチップのマップオブジェクト2Dに対する相対的な回転角度を設定する。
		@param	angle	相対的な回転角度
		*/
		void SetAngle(float angle);

		/**
		@brief このチップのマップオブジェクト2Dに対する相対的な拡大率を取得する。
		@return 相対的な拡大率
		*/
		Vector2DF GetScale() const;

		/**
		@brief　このチップのマップオブジェクト2Dに対する相対的な拡大率を設定する。
		@param	scale	相対的な拡大率
		*/
		void SetScale(Vector2DF scale);

		/**
		@brief	このチップを描画する際の中心座標を取得する。
		@return	描画の際の中心座標
		*/
		Vector2DF GetCenterPosition() const;

		/**
		@brief	このチップを描画する際の中心座標を取得する。
		@param	position	描画の際の中心座標
		*/
		void SetCenterPosition(Vector2DF position);

		/**
		@brief	このチップのテクスチャを描画する際に合成する色を取得する。
		@return	合成する色
		*/
		Color GetColor() const;

		/**
		@brief	このチップのテクスチャを描画する際に合成する色を設定する。
		@param	color	合成する色
		*/
		void SetColor(Color color);

		/**
		@brief　このチップのテクスチャ描画の左右反転フラグを取得する。
		@return	テクスチャ描画の左右反転フラグ
		*/
		bool GetTurnLR() const;

		/**
		@brief　このチップのテクスチャ描画の左右反転フラグを設定する。
		@param	turnLR	テクスチャ描画の左右反転フラグ
		*/
		void SetTurnLR(bool turnLR);

		/**
		@brief　このチップのテクスチャ描画の上下反転フラグを取得する。
		@return	テクスチャ描画の上下反転フラグ
		*/
		bool GetTurnUL() const;

		/**
		@brief　このチップのテクスチャ描画の上下反転フラグを設定する。
		@param	turnLR	テクスチャ描画の上下反転フラグ
		*/
		void SetTurnUL(bool turnUL);

		/**
		@brief	このチップの描画優先度を取得する。
		@return	描画優先度
		*/
		int GetDrawingPriority() const;

		/**
		@brief	このチップの描画優先度を設定する。
		@param	描画優先度
		*/
		void SetDrawingPriority(int priority);

		/**
		@brief　このチップのブレンディング モードを取得する。
		@return	ブレンディング モード
		*/
		AlphaBlendMode GetAlphaBlendMode() const;

		/**
		@brief　このチップのブレンディング モードを設定する。
		@param	alphaBlend	ブレンディング モード
		*/
		void SetAlphaBlendMode(AlphaBlendMode alphaBlend);

		/**
		@brief　このチップのテクスチャを描画する際のテクスチャフィルタの種類を設定する。
		@param	textureFilterType	テクスチャフィルタのタイプ
		*/
		void SetTextureFilterType(TextureFilterType textureFilterType);

		/**
		@brief　このチップのテクスチャを描画する際のテクスチャフィルタの種類を取得する。
		@return	テクスチャフィルタのタイプ
		*/
		TextureFilterType GetTextureFilterType() const;
	};
}