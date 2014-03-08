
#pragma once

#include "../ace.Core.Base.h"

namespace ace
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
		@brief	ループポイントを設定する。
		@param	loopStart	ループ戻り地点(秒)
		@param	loopEnd		ループ開始地点(秒)
		*/
		virtual void SetLoopPoint(float loopStart, float loopEnd) = 0;

		/**
		@brief	音の長さを取得する。
		@return	長さ(秒)
		*/
		virtual float GetLength() = 0;
	};
}