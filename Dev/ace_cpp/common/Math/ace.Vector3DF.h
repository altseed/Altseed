
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
	*/
	Vector3DF( float x, float y, float z );

	Vector3DF operator + ( const Vector3DF& o ) const;

	Vector3DF operator - ( const Vector3DF& o ) const;

	Vector3DF operator * ( const float& o ) const;

	Vector3DF operator / ( const float& o ) const;

	Vector3DF& operator += ( const Vector3DF& o );

	Vector3DF& operator -= ( const Vector3DF& o );

	Vector3DF& operator *= ( const float& o );

	Vector3DF& operator /= ( const float& o );

	bool operator == (const Vector3DF& o);
	bool operator != (const Vector3DF& o);

	/**
		@brief	加算
	*/
	static void Add( Vector3DF* pOut, const Vector3DF* pIn1, const Vector3DF* pIn2 );

	/**
		@brief	減算
	*/
	static Vector3DF& Sub( Vector3DF& o, const Vector3DF& in1, const Vector3DF& in2 );

	/**
		@brief	長さ
	*/
	static float Length( const Vector3DF& in );

	/**
		@brief	長さの二乗
	*/
	static float LengthSq( const Vector3DF& in );

	/**
		@brief	内積
	*/
	static float Dot( const Vector3DF& in1, const Vector3DF& in2 );

	/**
		@brief	単位ベクトル
	*/
	static void Normal( Vector3DF& o, const Vector3DF& in );

	/**
		@brief	外積
		@note
		右手系の場合、右手の親指がin1、人差し指がin2としたとき、中指の方向を返す。<BR>
		左手系の場合、左手の親指がin1、人差し指がin2としたとき、中指の方向を返す。<BR>
	*/
	static Vector3DF& Cross( Vector3DF& o, const Vector3DF& in1, const Vector3DF& in2 );

	static Vector3DF& Transform( Vector3DF& o, const Vector3DF& in, const Matrix44& mat );
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
 } 
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
