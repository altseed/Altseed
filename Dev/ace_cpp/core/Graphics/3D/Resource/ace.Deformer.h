
#pragma once

#include <Graphics/ace.Graphics.Common.h>
#include <Graphics/ace.Color.h>

#include <Math/ace.Vector2DI.h>
#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>

#include "../../../ace.Core.Base.h"
#include "../../../ace.ReferenceObject.h"


namespace ace
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
		virtual void AddBone(const achar* name, int32_t parentBoneIndex, eRotationOrder rotationOrder, const Matrix44& localMat) = 0;
	};
}