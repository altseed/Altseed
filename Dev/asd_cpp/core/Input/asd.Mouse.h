#pragma once

#include <memory>
#include "asd.MiddleButton.h"
#include "asd.SideButton.h"
#include "Math/asd.Vector2DF.h"
namespace asd{
	class Mouse
	{
	public:
		virtual SideButton* GetLeftButton() const = 0;


		virtual SideButton* GetRightButton() const = 0;


		virtual MiddleButton* GetMiddleButton() const = 0;


		virtual Vector2DF GetPosition() const = 0;

	};
};