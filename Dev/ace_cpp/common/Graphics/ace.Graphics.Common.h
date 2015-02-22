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
	enum class RotationOrder : int32_t
	{
		QUATERNION = 10,
		XZY = 11,
		XYZ = 12,
		ZXY = 13,
		ZYX = 14,
		YXZ = 15,
		YZX = 16,
		AXIS = 18,
	};

	/**
	@brief	キー間の補間方法
	*/
	enum class InterpolationType : int32_t
	{
		/**
		@brief	次のキーまで定数で補間
		*/
		Constant = 0,

		/**
		@brief	次のキーまで線形で補間
		*/
		Linear = 1,

		/**
		@brief	次のキーまで三次方程式で補間
		*/
		Cubic = 2,
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
		InterpolationType		InterpolationType;

		FCurveKeyframe()
		{
			InterpolationType = InterpolationType::Linear;
		}
	};


	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}