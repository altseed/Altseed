#pragma once

#include "asd.Joystick.h"

namespace asd{
	class JoystickContainer
	{
	public:
		JoystickContainer(){}
		virtual ~JoystickContainer(){}

		/**
		@brief	引数に指定した番号のジョイスティックが接続されているかを取得する。
		@param	at	何番目のジョイスティックの接続を確認するか
		@return 接続されているか否か
		*/
		virtual bool GetIsPresentAt(int at) = 0;

		/**
		@brief	引数に指定した番号のジョイスティッククラスを取得する。
		@param	at	何番目のジョイスティッククラスを取得するか
		@return 該当するジョイスティッククラス
		*/
		virtual Joystick* GetJoystickAt(int at) = 0;

		/**
		@brief	全てのジョイスティックの接続状態を更新する。
		*/
		virtual void RefreshAllJoysticks() = 0;
	};
};
