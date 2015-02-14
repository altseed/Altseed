#pragma once

#include "ace.Joystick.h"

namespace ace{
	class JoystickContainer
	{
	public:
		JoystickContainer(){}
		virtual ~JoystickContainer(){}

		/**
		@brief	引数に指定した番目のジョイスティックが接続されているかを返す
		@param	at	何番目のジョイスティックの接続を確認するか
		@return 接続されているか否か
		*/
		virtual const bool IsPresentAt(int at) = 0;

		/**
		@brief	引数に指定した番目のジョイスティッククラスを返す
		@param	at	何番目のジョイスティッククラスを取得するか
		@return 該当するジョイスティッククラス
		*/
		virtual Joystick* GetJoystickAt(int at) = 0;
	};
};
