#pragma once
#include "ace.Vector2DF.h"
#include "ace.Vector3DF.h"

namespace ace
{
	/**
		@brief	3×3行列を表す構造体。
		@note
		[0,0][0,1]
		[1,0][1,1]
		*/
	struct Matrix33
	{
	public:
		/**
			@brief	オブジェクトを生成し、単位行列で初期化する。
			*/
		Matrix33();

		/**
			@brief	行列の値
			*/
		float Values[3][3];

		/**
			@brief	単位行列を設定する。
			*/
		Matrix33& SetIdentity();

		/**
			@brief	平行移動の行列を設定する。
			*/
		Matrix33& SetTranslation(float x, float y);

		/**
			@brief	拡大・縮小行列を設定する。
			*/
		Matrix33& SetScaling(float x, float y);

		/**
			@brief	回転行列を設定する。
			*/
		Matrix33& SetRotation(float angle);

		/**
			@brief	3×3行列の掛け算を計算する。
			*/
		Matrix33 operator*(const Matrix33& right);

		Vector3DF operator*(const Vector3DF& right);
	};
}