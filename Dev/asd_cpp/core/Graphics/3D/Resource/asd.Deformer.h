
#pragma once

#include <Graphics/asd.Graphics.Common.h>
#include <Graphics/asd.Color.h>

#include <Math/asd.Vector2DI.h>
#include <Math/asd.Vector2DF.h>
#include <Math/asd.Vector3DF.h>
#include <Math/asd.Matrix44.h>

#include "../../../asd.Core.Base.h"
#include "../../../asd.ReferenceObject.h"


namespace asd
{
	/**
		@brief	メッシュを変形させるためのクラス
	*/
	class Deformer
		: public IReference
	{
	protected:
		Deformer() {}
		virtual ~Deformer() {}

	public:	
		/**
			@brief	ボーンを追加する。
			@param	name	ボーンの名称
			@param	parentBoneIndex	親ボーンのインデックス(親がない場合は-1)
			@param	rotationOrder	ボーンの回転行列の計算方法
			@param	localMat	ボーンのローカル変形行列
		*/
		virtual void AddBone(const achar* name, int32_t parentBoneIndex, RotationOrder rotationOrder, const Matrix44& localMat, const Vector3DF& translation, const Vector4DF& rotation, const Vector3DF& scaling) = 0;
	};
}