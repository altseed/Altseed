
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

		/**
		@brief	再生速度変更するかを取得する。
		@return	再生速度変更するか?
		*/
		virtual bool GetIsPlaybackSpeedEnabled() = 0;

		/**
		@brief	再生速度変更するかを設定する。
		@param	isPlaybackSpeedEnabled	再生速度変更するか?
		*/
		virtual void SetIsPlaybackSpeedEnabled(bool isPlaybackSpeedEnabled) = 0;

		/**
		@brief	再生速度を取得する。
		@return	再生速度(比率)
		@note
		設定値は再生速度に比例する。1.0で等速である。
		音程は再生速度に比例して変化する。
		*/
		virtual float GetPlaybackSpeed() = 0;

		/**
		@brief	再生速度を取得する。
		@param	playbackSpeed	再生速度(比率)
		@note
		設定値は再生速度に比例する。1.0で等速である。
		音程は再生速度に比例して変化する。
		*/
		virtual void SetPlaybackSpeed(float playbackSpeed) = 0;

		/**
		@brief	パン位置を取得する。
		@return	パン位置, 0.0で中央, -1.0で左, 1.0で右。
		*/
		virtual float GetPanningPosition() = 0;

		/**
		@brief	パン位置を設定する。
		@param	panningPosition	パン位置, 0.0で中央, -1.0で左, 1.0で右。
		*/
		virtual void SetPanningPosition(float panningPosition) = 0;

	};
}