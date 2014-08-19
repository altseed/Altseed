#pragma once
#include <memory>
#include "Color.h"

namespace FontGenerator
{
	struct BorderSetting
	{
		int32_t size;
		int32_t sampling;
		Color color;

		BorderSetting()
			: size(0), color(Color()), sampling(1)
		{
		}

		BorderSetting(float size, Color color, int32_t sampling)
			: size(size), color(color), sampling(1)
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