#pragma once

#include "ace.Shape.h"

namespace ace
{
	class Line :public Shape
	{
	private:
		std::shared_ptr<CoreLine> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;
	public:
		Line();
		virtual ~Line();

		/**
		@brief	線分の始点座標を取得する。
		@return	線分の視点座標
		*/
		Vector2DF GetStartingPosition() const;

		/**
		@brief	線分の視点座標を設定する。
		@param	startingPosition	線分の始点座標
		*/
		void SetStartingPosition(Vector2DF startingPosition);

		/**
		@brief	線分の終点座標を取得する。
		@return	線分の終点座標
		*/
		Vector2DF GetEndingPosition() const;

		/**
		@brief	線分の終点座標を設定する。
		@param	endingPosition	線分の終点座標
		*/
		void SetEndingPosition(Vector2DF endingPosition);

		/**
		@brief	線分の太さを取得する。
		@return	線分の太さ
		*/
		float GetThickness() const;

		/**
		@brief	線分の太さを設定する。
		@param	thickness	線分の太さ
		*/
		void SetThickness(float thickness);

		ShapeType GetShapeType() const override;
	};
}