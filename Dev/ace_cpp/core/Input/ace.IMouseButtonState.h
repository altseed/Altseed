#pragma once

namespace ace{

	enum class MouseButtonState:int
	{
		Push,Release,Hold,Free
	};

	class IMouseButtonState
	{
	public:
		virtual const MouseButtonState GetButtonState() const = 0;
	};
};