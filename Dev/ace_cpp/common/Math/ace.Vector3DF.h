
#pragma once

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "../ace.common.Base.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace { 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	3次元ベクトル
*/
struct Vector3DF
{
public:
	/**
		@brief	X
	*/
	float	X;

	/**
		@brief	Y
	*/
	float	Y;

	/**
		@brief	Z
	*/
	float	Z;

	/**
		@brief	コンストラクタ
	*/
	Vector3DF();

	/**
	@brief	コンストラクタ
	@param	x	X
	@param	y	Y
	@param	z	Z
	*/
	Vector3DF( float x, float y, float z );

	bool operator == (const Vector3DF& o);
	bool operator != (const Vector3DF& o);

	Vector3DF operator-();

	Vector3DF operator + ( const Vector3DF& o ) const;

	Vector3DF operator - ( const Vector3DF& o ) const;

	Vector3DF operator * (const Vector3DF& o) const;

	Vector3DF operator / (const Vector3DF& o) const;

	Vector3DF operator * ( const float& o ) const;

	Vector3DF operator / ( const float& o ) const;

	Vector3DF& operator += ( const Vector3DF& o );

	Vector3DF& operator -= ( const Vector3DF& o );

	Vector3DF& operator *= (const Vector3DF& o);

	Vector3DF& operator /= (const Vector3DF& o);

	Vector3DF& operator *= ( const float& o );

	Vector3DF& operator /= ( const float& o );


	/**
	@brief	このベクトルの長さを取得する。
	*/
	float GetLength() const
	{
		return sqrt(GetSquaredLength());
	}

	/**
	@brief	このベクトルの長さの二乗を取得する。
	*/
	float GetSquaredLength() const
	{
		return X * X + Y * Y + Z * Z;
	}

	/**
	@brief	このベクトルの長さを設定する。
	*/
	void SetLength(float value)
	{
		float length = GetLength();
		(*this) *= (value / length);
	}

	/**
	@brief	このベクトルの単位ベクトルを取得する。
	*/
	Vector3DF GetNormal()
	{
		float length = GetLength();
		return Vector3DF(X / length, Y / length, Z / length);
	}

	/**
	@brief	このベクトルの単位ベクトル化する。
	*/
	void Normalize()
	{
		float length = GetLength();
		(*this) /= length;
	}

	/**
		@brief	内積を取得する。
	*/
	static float Dot( const Vector3DF& v1, const Vector3DF& v2 );

	/**
		@brief	外積を取得する。
		@note
		右手系の場合、右手の親指がv1、人差し指がv2としたとき、中指の方向を返す。<BR>
		左手系の場合、左手の親指がv1、人差し指がv2としたとき、中指の方向を返す。<BR>
	*/
	static Vector3DF Cross(const Vector3DF& v1, const Vector3DF& v2 );

	/**
		@brief	2点間の距離を取得する。
	*/
	static float Distance(const Vector3DF& v1, const Vector3DF& v2);

	static Vector3DF& Transform( Vector3DF& o, const Vector3DF& in, const Matrix44& mat );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
