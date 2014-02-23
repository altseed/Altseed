#include "Color.h"

using namespace std;

namespace FontGenerator
{
	Color::Color()
		: Color(0, 0, 0, 255)
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

	Color Color::Blend(Color src)
	{
		Color result = *this;
		float srca = src.a / 255.0f;
		float dsta = a / 255.0f;
		float outa = srca + dsta * (1 - srca);

		result.r = (int)((src.r * srca + r * dsta * (1 - srca)) / outa);
		result.g = (int)((src.g * srca + g * dsta * (1 - srca)) / outa);
		result.b = (int)((src.b * srca + b * dsta * (1 - srca)) / outa);
		result.a = (int)(outa * 255);

		return result;
	}
}