#pragma once

namespace asd
{
	/**
		@brief	オブジェクトの親子関係の同期方法を表す列挙体
	*/
	enum class ChildMode : int32_t
	{
		/**
			@brief	位置のみを同期する。
		*/
		Position = 0,

		/**
			@brief	位置・角度・拡大率を同期する。
		*/
		All = 1,

		/**
			@brief	同期を行わない。
		*/
		Nothing = 2,
	};
}