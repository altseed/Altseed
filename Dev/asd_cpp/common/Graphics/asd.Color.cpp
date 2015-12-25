#include "asd.Color.h"

namespace asd {
	Color::Color()
		: A(255)
		, R(255)
		, G(255)
		, B(255)
	{
	}

	Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: A(a)
		, R(r)
		, G(g)
		, B(b)
	{
	
	}

	Color Color::operator*(const Color& right)
	{
		return Color(
			R * right.R / 255,
			G * right.G / 255,
			B * right.B / 255,
			A * right.A / 255);
	}

	Color& Color::operator*=(const Color& right)
	{
		R = R * right.R / 255;
		G = G * right.G / 255;
		B = B * right.B / 255;
		A = A * right.A / 255;
		return *this;
	}
}

