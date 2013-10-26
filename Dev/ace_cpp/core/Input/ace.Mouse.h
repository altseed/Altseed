#pragma once

#include <memory>
#include "ace.MiddleButton.h"
#include "ace.SideButton.h"
#include "Math/ace.Vector2DF.h"
namespace ace{
	class Mouse
	{
	public:
		virtual const SideButton* GetLeftButton() const = 0;


		virtual const SideButton* GetRightButton() const = 0;


		virtual const MiddleButton* GetMiddleButton() const = 0;


		virtual const Vector2DF GetPosition() const = 0;

	};
};