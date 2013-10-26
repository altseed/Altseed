#include "ace.Matrix33.h"

namespace ace
{
	Matrix33::Matrix33()
	{
		SetIdentity();
	}

	Matrix33& Matrix33::SetIdentity()
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				Values[i][j] = i == j ? 1 : 0;
			}
		}

		return *this;
	}

	Matrix33& Matrix33::SetTranslation(float x, float y)
	{
		SetIdentity();

		Values[0][2] = x;
		Values[1][2] = y;

		return *this;
	}

	Matrix33& Matrix33::SetScaling(float x, float y)
	{
		SetIdentity();

		Values[0][0] = x;
		Values[1][1] = y;
		return *this;
	}

	Matrix33& Matrix33::SetRotation(float angle)
	{
		SetIdentity();

		float sin, cos;
		SinCos(angle, sin, cos);

		Values[0][0] = cos;
		Values[0][1] = -sin;
		Values[1][0] = sin;
		Values[1][1] = cos;

		return *this;
	}

	Matrix33 Matrix33::operator*(const Matrix33& right)
	{
		Matrix33 result;

		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				result.Values[i][j] = 0;
				for (int k = 0; k < 3; ++k)
				{
					result.Values[i][j] += Values[i][k] * right.Values[k][j];
				}
			}
		}

		return result;
	}

	Vector3DF Matrix33::operator*(const Vector3DF& right)
	{
		float elements[3] = {0, 0, 0};
		float rop[3] = {right.X, right.Y, right.Z};

		for (int i = 0; i < 3; ++i)
		{
			for (int k = 0; k < 3; ++k)
			{
				elements[i] += Values[i][k] * rop[k];
			}
		}

		Vector3DF result;
		result.X = elements[0];
		result.Y = elements[1];
		result.Z = elements[2];

		return result;
	}
}