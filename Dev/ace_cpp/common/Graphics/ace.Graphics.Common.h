#pragma once

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "../ace.common.Base.h"
#include "../Math/ace.Vector2DF.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	/**
	@brief	回転行列の計算順序
	*/
	enum eRotationOrder
	{
		ROTATION_ORDER_QUATERNION = 10,
		ROTATION_ORDER_XZY = 11,
		ROTATION_ORDER_XYZ = 12,
		ROTATION_ORDER_ZXY = 13,
		ROTATION_ORDER_ZYX = 14,
		ROTATION_ORDER_YXZ = 15,
		ROTATION_ORDER_YZX = 16,
		ROTATION_ORDER_AXIS = 18,
		ROTATION_ORDER_MAX = 0xfffffff
	};

	/**
	@brief	キー間の補間方法
	*/
	enum eInterpolationType
	{
		/**
		@brief	次のキーまで定数で補間
		*/
		INTERPOLATION_TYPE_CONSTANT = 0,

		/**
		@brief	次のキーまで線形で補間
		*/
		INTERPOLATION_TYPE_LINEAR = 1,

		/**
		@brief	次のキーまで三次方程式で補間
		*/
		INTERPOLATION_TYPE_CUBIC = 2,

		/**
		@brief	int型指定
		*/
		INTERPOLATION_TYPE_MAX = 0xfffffff
	};

	/**
	@brief	Fカーブのキーフレーム
	*/
	struct FCurveKeyframe
	{
	public:
		/**
		@brief	時間と値
		*/
		Vector2DF				KeyValue;

		/**
		@brief	制御用ハンドル
		*/
		Vector2DF				LeftHandle;

		/**
		@brief	制御用ハンドル
		*/
		Vector2DF				RightHandle;

		/**
		@brief	補間方法
		*/
		eInterpolationType		InterpolationType;

		FCurveKeyframe()
		{
			InterpolationType = eInterpolationType::INTERPOLATION_TYPE_LINEAR;
		}
	};


	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}