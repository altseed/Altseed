
#pragma once

#include "../../../../asd.Core.Base.h"
#include "../../../../asd.ReferenceObject.h"

namespace asd
{
	/**
		@brief	アニメーションの元データのクラス
	*/
	class AnimationSource
		: public IReference
	{
	public:
		AnimationSource() {}
		virtual ~AnimationSource() {}

		/**
			@brief	ボーンごとのアニメーションを追加する。
			@param	keyframeAnimation	アニメーション
		*/
		virtual void AddAnimation(KeyframeAnimation* keyframeAnimation) = 0;

		/**
		@brief	ボーンごとのアニメーションを削除する。
		@param	keyframeAnimation	アニメーション
		*/
		virtual void RemoveAnimation(KeyframeAnimation* keyframeAnimation) = 0;

		/**
		@brief	アニメーションの長さ(60フレーム単位)を取得する。
		@return	アニメーションの長さ
		*/
		virtual float GetLength() const = 0;

#if !SWIG
		/**
		@brief	ボーンごとのアニメーションを追加する。
		@param	keyframeAnimation	アニメーション
		*/
		void AddAnimation(std::shared_ptr<KeyframeAnimation> keyframeAnimation)
		{
			AddAnimation(keyframeAnimation.get());
		}

		/**
		@brief	ボーンごとのアニメーションを削除する。
		@param	keyframeAnimation	アニメーション
		*/
		void RemoveAnimation(std::shared_ptr<KeyframeAnimation> keyframeAnimation)
		{
			RemoveAnimation(keyframeAnimation.get());
		}
#endif
	};
}

