#pragma once

#include <cstdint>

namespace ace
{
	/**
	@brief マウスボタンの押し状態を示す列挙型
	*/
	enum class MouseButtonState : int32_t
	{
		/**
		@brief	ボタンをこのフレームで押した状態
		*/
		Push,

		/**
		@brief	ボタンをこのフレームで離した状態
		*/
		Release,

		/**
		@brief	ボタンを離し続けている状態
		*/
		Free,

		/**
		@brief	ボタンを押し続けている状態
		*/
		Hold
	};
};