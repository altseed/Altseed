#pragma once
#include <cstdint>

namespace FontGenerator
{
	struct Color
	{
	public:
		Color();
		Color( std::uint8_t r,
			std::uint8_t g,
			std::uint8_t b,
			std::uint8_t a);
		Color(std::int32_t color);

		std::uint8_t a, r, g, b;

		std::int32_t GetInt() const;

		Color Blend(Color c);
	};
}