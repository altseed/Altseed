
#pragma once

#include <ace.common.Base.h>

#include "../../../ace.Core.Base.h"

#include "../../../ace.ReferenceObject.h"


namespace ace
{
	/**
	@brief	大量描画が可能な3Dモデルクラス
	*/
	class MassModel
		: public IReference
	{
	private:

	protected:
		MassModel(){}
		virtual ~MassModel(){}
	public:

		/**
		@brief	モデルが持つアニメーションの個数を取得する。
		@return	アニメーションの個数
		*/
		virtual int32_t GetAnimationCount() const = 0;

		/**
		@brief	モデルが持つアニメーションの名称を取得する。
		@param	index	アニメーションのインデックス
		@return	アニメーションの名称
		*/
		virtual const achar* GetAnimationName(int32_t index) const = 0;

		/**
		@brief	モデルが持つアニメーションの長さ(60フレーム単位)を取得する。
		@param	name	アニメーション名
		@return	アニメーションの長さ
		*/
		virtual float GetAnimationLength(const achar* name) const = 0;

		/**
		@brief	アニメーションがループするかを取得する。
		@param	name	アニメーション名
		@return	ループするか?
		*/
		virtual bool GetIsAnimationLoopingMode(const achar* name) const = 0;

		/**
		@brief	アニメーションがループするかを設定する。
		@param	name	アニメーション名
		@return	isLoopingMode	ループするか?
		*/
		virtual void SetIsAnimationLoopingMode(const achar* name, bool isLoopingMode) = 0;

		/**
		@brief	材質を設定する。
		@param	material		材質
		*/
		virtual void SetMaterial(Material3D* material) = 0;

#if !SWIG
		/**
		@brief	材質を設定する。
		@param	material		材質
		*/
		void SetMaterial(std::shared_ptr<Material3D> material)
		{
			SetMaterial(material.get());
		}
#endif
	};
}