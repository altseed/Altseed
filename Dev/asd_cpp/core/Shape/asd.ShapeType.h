#pragma once

namespace asd
{
	/**
	@brief	図形の種類
	*/
	enum class ShapeType
	{
		/**
		@brief	円
		*/
		CircleShape,

		/**
		@brief	弧
		*/
		ArcShape,

		/**
		@brief	線分
		*/
		LineShape,

		/**
		@brief	矩形
		*/
		RectangleShape,

		/**
		@brief	三角形
		*/
		TriangleShape,

		/**
		@brief	多角形
		*/
		PolygonShape
	};
}