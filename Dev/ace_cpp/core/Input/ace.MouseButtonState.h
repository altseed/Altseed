#pragma once

namespace ace
{
	/**
	@brief マウスボタンの押し状態を示す列挙型
	*/
	enum class MouseButtonState :int
	{
		Push,
		Release,
		Free,
		Hold
	};
};