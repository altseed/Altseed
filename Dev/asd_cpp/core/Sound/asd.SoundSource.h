
#pragma once

#include "../asd.Core.Base.h"

namespace asd
{
	/**
	@brief	音源のクラス
	*/
	class SoundSource
		: public IReference
	{
	protected:
		SoundSource() {}
		~SoundSource() {}

	public:

		/**
		@brief	ループポイントの開始地点(秒)を取得する。
		@return	開始地点(秒)
		*/
		virtual float GetLoopStartingPoint() const = 0;

		/**
		@brief	ループポイントの開始地点(秒)を設定する。
		@param	startingPoint	開始地点(秒)
		*/
		virtual void SetLoopStartingPoint(float startingPoint) = 0;

		/**
		@brief	ループポイントの終了地点(秒)を取得する。
		@return	終了地点(秒)
		*/
		virtual float GetLoopEndPoint() const = 0;

		/**
		@brief	ループポイントの終了地点(秒)を設定する。
		@param	startingPoint	終了地点(秒)
		*/
		virtual void SetLoopEndPoint(float endPoint) = 0;

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

		/**
		@brief	音の長さを取得する。
		@return	長さ(秒)
		*/
		virtual float GetLength() = 0;
	};
}