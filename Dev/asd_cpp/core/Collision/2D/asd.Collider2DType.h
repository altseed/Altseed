#pragma once


namespace asd
{
	/**
	@brief	2Dコライダの形状タイプ
	*/
	enum class Collider2DShapeType{

		/**
		@brief	円形
		*/
		Circle,

		/**
		@brief	矩形
		*/
		Rectangle,

		/**
		@brief	線形
		*/
		Line,

		/**
		@brief	ポリゴン
		*/
		Polygon,
	};
}