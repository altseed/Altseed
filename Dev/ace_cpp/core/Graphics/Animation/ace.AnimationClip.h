
#pragma once

#include "../../ace.Core.Base.h"
#include "../../ace.ReferenceObject.h"

namespace ace
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