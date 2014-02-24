#pragma once

#include <array>
#include "../ace.common.Base.h"
#include "ace.Vector2DI.h"

namespace ace
{
	/**
		@brief	長方形(整数)の構造体
	*/
	struct RectI
	{
	public:
		/**
			@brief	左上X座標
		*/
		int X;

		/**
			@brief	左上Y座標
		*/
		int Y;

		/**
			@brief	横幅
		*/
		int Width;
		
		/**
			@brief	縦幅
		*/
		int Height;

		/**
			@brief	コンストラクタ
		*/
		RectI();

		/**
			@brief	コンストラクタ
			@param	x	左上X座標
			@param	y	左上Y座標
			@param	width	横幅
			@param	height	縦幅
		*/
		RectI(int x, int y, int width, int height);

		/**
			@brief	左上座標を取得する。
			@return	左上座標
		*/
		Vector2DI GetPosition() const;

		/**
			@brief	大きさを取得する。
			@return	大きさ
		*/
		Vector2DI GetSize() const;

		/**
			@brief	四隅の座標を(X,Y)、(X+Width,Y)、(X+Width,Y+Height)、(X,Y+Height)の順に配列として取得する。
			@return	座標
		*/
		std::array<Vector2DI, 4> GetVertexes() const;
	};
}