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

	Matrix33& Matrix33::SetTransposed()
	{
		for (int32_t c = 0; c < 3; c++)
		{
			for (int32_t r = 0; r < 3; r++)
			{
				float v = Values[r][c];
				Values[r][c] = Values[c][r];
				Values[c][r] = v;
			}
		}

		return *this;
	}

	Matrix33& Matrix33::SetInverted()
	{
		float a11 = this->Values[0][0];
		float a12 = this->Values[0][1];
		float a13 = this->Values[0][2];
		float a21 = this->Values[1][0];
		float a22 = this->Values[1][1];
		float a23 = this->Values[1][2];
		float a31 = this->Values[2][0];
		float a32 = this->Values[2][1];
		float a33 = this->Values[2][2];

		/* 行列式の計算 */
		float b11 = +a22 * a33 - a23 * a32;
		float b12 = +a13 * a32 - a12 * a33;
		float b13 = +a12 * a23 - a13 * a22;

		float b21 = +a23 * a31 - a21 * a33;
		float b22 = +a11 * a33 - a13 * a31;
		float b23 = +a13 * a21 - a11 * a23;

		float b31 = +a21 * a32 - a22 * a31;
		float b32 = +a12 * a31 - a11 * a32;
		float b33 = +a11 * a22 - a12 * a21;

		// 行列式の逆数をかける
		float Det =
			a11 * a22 * a33 + a21 * a32 * a13 + a31 * a12 * a23 - a11 * a32 * a23 - a31 * a22 * a13 - a21 * a12 * a33;
		if ((-FLT_MIN <= Det) && (Det <= +FLT_MIN))
		{
			return *this;
		}

		float InvDet = 1.0f / Det;

		Values[0][0] = b11 * InvDet;
		Values[0][1] = b12 * InvDet;
		Values[0][2] = b13 * InvDet;
		Values[1][0] = b21 * InvDet;
		Values[1][1] = b22 * InvDet;
		Values[1][2] = b23 * InvDet;
		Values[2][0] = b31 * InvDet;
		Values[2][1] = b32 * InvDet;
		Values[2][2] = b33 * InvDet;

		return *this;
	}

	Matrix33 Matrix33::GetInverted()
	{
		Matrix33 o = (*this);
		o.SetInverted();
		return o;
	}

	Matrix33& Matrix33::SetTranslation(float x, float y)
	{
		SetIdentity();

		Values[0][2] = x;
		Values[1][2] = y;

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

	Matrix33& Matrix33::SetScale(float x, float y)
	{
		SetIdentity();

		Values[0][0] = x;
		Values[1][1] = y;
		return *this;
	}

	Vector2DF Matrix33::Transform2D(const Vector2DF& in) const
	{
		float values[2];

		for (int i = 0; i < 3; i++)
		{
			values[i] = 0;
			values[i] += in.X * Values[i][0];
			values[i] += in.Y * Values[i][1];
			values[i] += 1.0f * Values[i][2];
		}

		Vector2DF o;
		o.X = values[0];
		o.Y = values[1];
		return o;
	}

	Vector3DF Matrix33::Transform3D(const Vector3DF& in) const
	{
		float values[3];

		for (int i = 0; i < 3; i++)
		{
			values[i] = 0;
			values[i] += in.X * Values[i][0];
			values[i] += in.Y * Values[i][1];
			values[i] += in.Z * Values[i][2];
		}

		Vector3DF o;
		o.X = values[0];
		o.Y = values[1];
		o.Z = values[2];
		return o;
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