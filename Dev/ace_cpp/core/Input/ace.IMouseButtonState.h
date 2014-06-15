#pragma once

namespace ace{

	enum class MouseButtonState:int
	{
		Push,Pull,Hold,Free
	};

	class IMouseButtonState
	{
	public:
		virtual const MouseButtonState GetButtonState() const = 0;
	};
};