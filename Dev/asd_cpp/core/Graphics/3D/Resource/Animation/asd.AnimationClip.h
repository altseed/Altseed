﻿
#pragma once

#include "../../../../asd.Core.Base.h"
#include "../../../../asd.ReferenceObject.h"

namespace asd
{
	/**
		@brief	アニメーションソースを元に情報が付加されたアニメーションクラス
	*/
	class AnimationClip
		: public IReference
	{
		friend class Accessor;

	protected:
		virtual AnimationSource* GetSource_() = 0;
	public:

		AnimationClip() {}
		virtual ~AnimationClip() {}

		/**
			@brief	ソースを設定する。
			@param	source	ソース
		*/
		virtual void SetSource(AnimationSource* source) = 0;

		/**
		@brief	ループするかを取得する。
		@return	ループするか?
		*/
		virtual bool GetIsLoopingMode() const = 0;

		/**
		@brief	ループするかを設定する。
		@return	isLoopingMode	ループするか?
		*/
		virtual void SetIsLoopingMode(bool isLoopingMode) = 0;

#if !SWIG
		/**
		@brief	ソースを取得する。
		@return	source	ソース
		*/
		std::shared_ptr<AnimationSource> GetSource()
		{
			auto o = GetSource_();
			SafeAddRef(o);
			return CreateSharedPtrWithReleaseDLL(o);
		}

		/**
		@brief	ソースを設定する。
		@param	source	ソース
		*/
		void SetSource(std::shared_ptr<AnimationSource>& source)
		{
			SetSource(source.get());
		}
#endif
	};
}