#pragma once
#include <memory>
#include "Color.h"

namespace FontGenerator
{
	struct BorderSetting
	{
		float width;
		Color color;

		BorderSetting()
			: width(0), color(Color())
		{
		}

		BorderSetting(float width, Color color)
			: width(width), color(color)
		{
		}

		typedef std::shared_ptr<BorderSetting> Ptr;
	};
	struct BoldSetting
	{
		float width;

		BoldSetting()
			: width(1)
		{
		}

		BoldSetting(float width)
			: width(width)
		{
		}

		typedef std::shared_ptr<BoldSetting> Ptr;
	};
}