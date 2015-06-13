#pragma once

#include "asd.Shape.h"

namespace asd
{
	/**
	@brief	図形としての矩形を扱うクラス。
	*/
	class RectangleShape :public Shape
	{
	private:
		std::shared_ptr<CoreRectangleShape> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;
	public:
		RectangleShape();
		virtual ~RectangleShape();

		/**
		@brief	矩形の描画範囲を取得する。
		@return	矩形の描画範囲
		*/
		RectF GetDrawingArea() const;

		/**
		@brief	矩形の描画範囲を設定する。
		@param	drawingArea	矩形の描画範囲
		*/
		void SetDrawingArea(RectF drawingArea);

		/**
		@brief	矩形の頂点別UVを取得する。
		@return	矩形の頂点別UV
		*/
		RectF GetUV() const;

		/**
		@brief	矩形の頂点別UVを設定する。
		@param	uv	矩形の頂点別UV
		*/
		void SetUV(RectF uv);

		/**
		@brief	矩形の回転角度を取得する。
		@return	矩形の回転角度
		*/
		float GetAngle() const;

		/**
		@brief	矩形の回転角度を設定する。
		@param	angle	矩形の回転角度
		*/
		void SetAngle(float angle);

		/**
		@brief	矩形の回転の際の中心座標を取得する。
		@return	矩形の回転の際の中心座標
		*/
		Vector2DF GetCenterPosition() const;

		/**
		@brief	矩形の回転の際の中心座標を設定する。
		@centerPosition	矩形の回転の際の中心座標
		*/
		void SetCenterPosition(Vector2DF centerPosition);

		ShapeType GetShapeType() const override;
	};
}