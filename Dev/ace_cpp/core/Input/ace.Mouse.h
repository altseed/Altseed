#pragma once

#include <memory>
#include "ace.MiddleButton.h"
#include "ace.SideButton.h"
#include "Math/ace.Vector2DF.h"
namespace ace{
	class Mouse
	{
	public:
		virtual SideButton* GetLeftButton() const = 0;


		virtual SideButton* GetRightButton() const = 0;


		virtual MiddleButton* GetMiddleButton() const = 0;


		virtual Vector2DF GetPosition() const = 0;

	};
};