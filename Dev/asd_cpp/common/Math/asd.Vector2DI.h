
#pragma once

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "../asd.common.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace asd
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------

	/**
		@brief	2次元ベクトル(整数)
		*/
	struct Vector2DI
	{
	public:
		/**
			@brief	X
			*/
		int32_t	X;

		/**
			@brief	Y
			*/
		int32_t	Y;

		/**
			@brief	コンストラクタ
			*/
		Vector2DI();

		/**
			@brief	コンストラクタ
			*/
		Vector2DI(int32_t x, int32_t y);

		bool operator == (const Vector2DI& o);
		bool operator != (const Vector2DI& o);

		Vector2DI operator-();

		Vector2DI operator+(const Vector2DI& right);

		Vector2DI operator-(const Vector2DI& right);

		Vector2DI operator*(const Vector2DI& right);

		Vector2DI operator/(const Vector2DI& right);

		Vector2DI operator*(int32_t right);

		Vector2DI operator/(int32_t right);

		Vector2DI& operator+=(const Vector2DI& right);

		Vector2DI& operator-=(const Vector2DI& right);

		Vector2DI& operator*=(const Vector2DI& right);

		Vector2DI& operator/=(const Vector2DI& right);

		Vector2DI& operator*=(int32_t right);

		Vector2DI& operator/=(int32_t right);

		/**
		@brief	Vector2DF型に変換する。
		@return	Vector2DF型
		*/
		Vector2DF To2DF() const;

		/**
			@brief	スカラーで除算する。
			@param	v1	値1
			@param	v2	値2
			@return	v1/v2
		*/
		static Vector2DI DivideByScalar(const Vector2DI& v1, float v2);
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
