#include "Color.h"

using namespace std;

namespace FontGenerator
{
	Color::Color()
		: Color(0, 0, 0, 0)
	{
	}

	Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: r(r), g(g), b(b), a(a)
	{
	}

	Color::Color(int32_t color)
	{
		r = (color & (0xff << 0)) >> 0;
		g = (color & (0xff << 8)) >> 8;
		b = (color & (0xff << 16)) >> 16;
		a = (color & (0xff << 24)) >> 24;
	}

	int32_t Color::GetInt() const
	{
		int32_t result = 0;
		result += (int)r << 0;
		result += (int)g << 8;
		result += (int)b << 16;
		result += (int)a << 24;
		return result;
	}
}