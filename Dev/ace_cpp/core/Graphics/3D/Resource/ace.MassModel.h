
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
		@brief	ループするかを取得する。
		@param	name	アニメーション名
		@return	ループするか?
		*/
		virtual bool GetIsLoopingMode(const achar* name) const = 0;

		/**
		@brief	ループするかを設定する。
		@param	name	アニメーション名
		@return	isLoopingMode	ループするか?
		*/
		virtual void SetIsLoopingMode(const achar* name, bool isLoopingMode) = 0;

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