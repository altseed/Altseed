#pragma once

namespace ace
{
	/**
	@brief	図形の種類
	*/
	enum class ShapeType
	{
		/**
		@brief	円
		*/
		Circle,

		/**
		@brief	弧
		*/
		Arc,

		/**
		@brief	線分
		*/
		Line,

		/**
		@brief	矩形
		*/
		Rectangle,

		/**
		@brief	三角形
		*/
		Triangle,

		/**
		@brief	多角形
		*/
		PolygonShape
	};
}