﻿
#pragma once

#include "../../../../asd.Core.Base.h"
#include "../../../../asd.ReferenceObject.h"

#include <Math/asd.Vector2DF.h>
#include <Graphics/asd.Graphics.Common.h>

namespace asd
{
	/**
		@brief	キーフレームで指定するアニメーションのクラス
	*/
	class KeyframeAnimation
		: public IReference
	{
	protected:
		KeyframeAnimation() {}
		virtual ~KeyframeAnimation() {}

	public:
		/**
			@brief	名称を取得する。
			@return	名称
		*/
		virtual const achar* GetName() = 0;

		/**
			@brief	名称を指定する。
			@param	name	名称
		*/
		virtual void SetName(const achar* name) = 0;

		/**
			@brief	キーフレームを追加する。
			@param	kf	キーフレーム
		*/
		virtual void AddKeyframe(const FCurveKeyframe& kf) = 0;

		/**
			@brief	指定した時間の値を取得する。
			@param	time	時間
			@return	値
		*/
		virtual float GetValue(float time) = 0;
	};
}