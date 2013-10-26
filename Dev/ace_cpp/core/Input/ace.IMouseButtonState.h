#pragma once

namespace ace{

	enum eMouseButtonState
	{
		MOUSE_PUSH,MOUSE_PULL,MOUSE_HOLD,MOUSE_FREE
	};

	class IMouseButtonState
	{
	public:
		virtual eMouseButtonState GetButtonState() const = 0;
	};
};