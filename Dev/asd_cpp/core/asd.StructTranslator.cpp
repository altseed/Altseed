
#include "asd.StructTranslator.h"

namespace asd
{
	StructTranslator::Buffer StructTranslator::buffers [] = { 0 };
	int32_t StructTranslator::size = 0;
	int32_t StructTranslator::currentIndex = 0;

	void StructTranslator::EnqueueVector2DF(float x, float y)
	{
		size += 2;
		buffers[size - 2].Float = x;
		buffers[size - 1].Float = y;
	}

	void StructTranslator::EnqueueVector3DF(float x, float y, float z)
	{
		size += 3;
		buffers[size - 3].Float = x;
		buffers[size - 2].Float = y;
		buffers[size - 1].Float = z;
	}

	void StructTranslator::EnqueueVector4DF(float x, float y, float z, float w)
	{
		size += 4;
		buffers[size - 4].Float = x;
		buffers[size - 3].Float = y;
		buffers[size - 2].Float = z;
		buffers[size - 1].Float = w;
	}

	void StructTranslator::EnqueueVector2DI(int32_t x, int32_t y)
	{
		size += 2;
		buffers[size - 2].Int = x;
		buffers[size - 1].Int = y;
	}

	void StructTranslator::EnqueueColor(int32_t r, int32_t g, int32_t b, int32_t a)
	{
		size += 4;
		buffers[size - 4].Int = r;
		buffers[size - 3].Int = g;
		buffers[size - 2].Int = b;
		buffers[size - 1].Int = a;
	}

	void StructTranslator::EnqueueRectF(float x, float y, float w, float h)
	{
		size += 4;
		buffers[size - 4].Float = x;
		buffers[size - 3].Float = y;
		buffers[size - 2].Float = w;
		buffers[size - 1].Float = h;
	}

	void StructTranslator::EnqueueRectI(int32_t x, int32_t y, int32_t w, int32_t h)
	{
		size += 4;
		buffers[size - 4].Int = x;
		buffers[size - 3].Int = y;
		buffers[size - 2].Int = w;
		buffers[size - 1].Int = h;
	}

	void StructTranslator::EnqueueMatrix44(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33)
	{
		size += 16;
		buffers[size - 16].Float = m00;
		buffers[size - 15].Float = m01;
		buffers[size - 14].Float = m02;
		buffers[size - 13].Float = m03;
		buffers[size - 12].Float = m10;
		buffers[size - 11].Float = m11;
		buffers[size - 10].Float = m12;
		buffers[size -  9].Float = m13;
		buffers[size -  8].Float = m20;
		buffers[size -  7].Float = m21;
		buffers[size -  6].Float = m22;
		buffers[size -  5].Float = m23;
		buffers[size -  4].Float = m30;
		buffers[size -  3].Float = m31;
		buffers[size -  2].Float = m32;
		buffers[size -  1].Float = m33;
	}

	void StructTranslator::EnqueueFCurveKeyframe(
		float kv_x, float kv_y,
		float lh_x, float lh_y,
		float rh_x, float rh_y,
		int32_t i)
	{
		size += 7;
		buffers[size - 7].Float = kv_x;
		buffers[size - 6].Float = kv_y;
		buffers[size - 5].Float = lh_x;
		buffers[size - 4].Float = lh_y;
		buffers[size - 3].Float = rh_x;
		buffers[size - 2].Float = rh_y;
		buffers[size - 1].Int = i;
	}

	float StructTranslator::DequeueFloat()
	{
		auto ret = buffers[currentIndex].Float;
		currentIndex++;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return ret;
	}

	int32_t StructTranslator::DequeueInt()
	{
		auto ret = buffers[currentIndex].Int;
		currentIndex++;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return ret;
	}

	void StructTranslator::EnqueueVector2DF(const Vector2DF& v)
	{
		size += 2;
		buffers[size - 2].Float = v.X;
		buffers[size - 1].Float = v.Y;
	}

	void StructTranslator::EnqueueVector3DF(const Vector3DF& v)
	{
		size += 3;
		buffers[size - 3].Float = v.X;
		buffers[size - 2].Float = v.Y;
		buffers[size - 1].Float = v.Z;
	}

	void StructTranslator::EnqueueVector4DF(const Vector4DF& v)
	{
		size += 4;
		buffers[size - 4].Float = v.X;
		buffers[size - 3].Float = v.Y;
		buffers[size - 2].Float = v.Z;
		buffers[size - 1].Float = v.W;
	}

	void StructTranslator::EnqueueVector2DI(const Vector2DI& v)
	{
		size += 2;
		buffers[size - 2].Int = v.X;
		buffers[size - 1].Int = v.Y;
	}

	void StructTranslator::EnqueueColor(const Color& v)
	{
		size += 4;
		buffers[size - 4].Int = v.R;
		buffers[size - 3].Int = v.G;
		buffers[size - 2].Int = v.B;
		buffers[size - 1].Int = v.A;
	}

	void StructTranslator::EnqueueRectF(const RectF& v)
	{
		size += 4;
		buffers[size - 4].Float = v.X;
		buffers[size - 3].Float = v.Y;
		buffers[size - 2].Float = v.Width;
		buffers[size - 1].Float = v.Height;
	}

	void StructTranslator::EnqueueRectI(const RectI& v)
	{
		size += 4;
		buffers[size - 4].Int = v.X;
		buffers[size - 3].Int = v.Y;
		buffers[size - 2].Int = v.Width;
		buffers[size - 1].Int = v.Height;
	}

	void StructTranslator::EnqueueMatrix33(const Matrix33& v)
	{
		size += 9;
		memcpy(&(buffers[size - 9].Float), &v, sizeof(float) * 9);
	}

	void StructTranslator::EnqueueMatrix44(const Matrix44& v)
	{
		size += 16;
		memcpy(&(buffers[size - 16].Float), &v, sizeof(float) * 16);
	}

	void StructTranslator::EnqueueFCurveKeyframe(const FCurveKeyframe& v)
	{
		size += 7;
		memcpy(&(buffers[size - 7].Float), &v, sizeof(float) * 7);
	}

	Vector2DF StructTranslator::DequeueVector2DF()
	{
		auto ret0 = buffers[currentIndex+0].Float;
		auto ret1 = buffers[currentIndex+1].Float;

		currentIndex+=2;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return Vector2DF(ret0, ret1);
	}

	Vector2DF* StructTranslator::Dequeue_Vector2DF()
	{
		auto ret = (Vector2DF*)&(buffers[currentIndex + 0].Float);
		currentIndex += 2;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return ret;
	}

	Vector3DF StructTranslator::DequeueVector3DF()
	{
		auto ret0 = buffers[currentIndex + 0].Float;
		auto ret1 = buffers[currentIndex + 1].Float;
		auto ret2 = buffers[currentIndex + 2].Float;

		currentIndex += 3;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return Vector3DF(ret0, ret1, ret2);
	}

	Vector3DF* StructTranslator::Dequeue_Vector3DF()
	{
		auto ret = (Vector3DF*) &(buffers[currentIndex + 0].Float);
		currentIndex += 3;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return ret;
	}

	Vector4DF StructTranslator::DequeueVector4DF()
	{
		auto ret0 = buffers[currentIndex + 0].Float;
		auto ret1 = buffers[currentIndex + 1].Float;
		auto ret2 = buffers[currentIndex + 2].Float;
		auto ret3 = buffers[currentIndex + 3].Float;

		currentIndex += 4;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return Vector4DF(ret0, ret1, ret2, ret3);
	}

	Vector4DF* StructTranslator::Dequeue_Vector4DF()
	{
		auto ret = (Vector4DF*) &(buffers[currentIndex + 0].Float);
		currentIndex += 4;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return ret;
	}

	Vector2DI StructTranslator::DequeueVector2DI()
	{
		auto ret0 = buffers[currentIndex + 0].Int;
		auto ret1 = buffers[currentIndex + 1].Int;

		currentIndex += 2;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return Vector2DI(ret0, ret1);
	}

	Vector2DI* StructTranslator::Dequeue_Vector2DI()
	{
		auto ret = (Vector2DI*) &(buffers[currentIndex + 0].Int);
		currentIndex += 2;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return ret;
	}

	Color StructTranslator::DequeueColor()
	{
		auto ret0 = buffers[currentIndex + 0].Int;
		auto ret1 = buffers[currentIndex + 1].Int;
		auto ret2 = buffers[currentIndex + 2].Int;
		auto ret3 = buffers[currentIndex + 3].Int;

		currentIndex += 4;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return Color(ret0, ret1, ret2, ret3);
	}

	Color* StructTranslator::Dequeue_Color()
	{
		auto ret = (Color*) &(buffers[currentIndex + 0].Int);

		auto ret0 = buffers[currentIndex + 0].Int;
		auto ret1 = buffers[currentIndex + 1].Int;
		auto ret2 = buffers[currentIndex + 2].Int;
		auto ret3 = buffers[currentIndex + 3].Int;

		ret->R = ret0;
		ret->G = ret1;
		ret->B = ret2;
		ret->A = ret3;

		currentIndex += 4;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return ret;
	}

	RectF StructTranslator::DequeueRectF()
	{
		auto ret0 = buffers[currentIndex + 0].Float;
		auto ret1 = buffers[currentIndex + 1].Float;
		auto ret2 = buffers[currentIndex + 2].Float;
		auto ret3 = buffers[currentIndex + 3].Float;

		currentIndex += 4;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return RectF(ret0, ret1, ret2, ret3);
	}

	RectF* StructTranslator::Dequeue_RectF()
	{
		auto ret = (RectF*) &(buffers[currentIndex + 0].Float);
		currentIndex += 4;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return ret;
	}

	RectI StructTranslator::DequeueRectI()
	{
		auto ret0 = buffers[currentIndex + 0].Int;
		auto ret1 = buffers[currentIndex + 1].Int;
		auto ret2 = buffers[currentIndex + 2].Int;
		auto ret3 = buffers[currentIndex + 3].Int;

		currentIndex += 4;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return RectI(ret0, ret1, ret2, ret3);
	}

	RectI* StructTranslator::Dequeue_RectI()
	{
		auto ret = (RectI*) &(buffers[currentIndex + 0].Int);
		currentIndex += 4;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return ret;
	}

	Matrix44 StructTranslator::DequeueMatrix44()
	{
		Matrix44 mat;
		memcpy(&mat, &(buffers[currentIndex + 0]), sizeof(float) * 16);
		
		currentIndex += 16;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return mat;
	}

	Matrix44* StructTranslator::Dequeue_Matrix44()
	{
		auto ret = (Matrix44*) &(buffers[currentIndex + 0].Float);
		currentIndex += 16;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return ret;
	}

	FCurveKeyframe StructTranslator::DequeueFCurveKeyframe()
	{
		FCurveKeyframe f;
		memcpy(&f, &(buffers[currentIndex + 0]), sizeof(float) * 7);

		currentIndex += 7;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return f;
	}

	FCurveKeyframe* StructTranslator::Dequeue_FCurveKeyframe()
	{
		auto ret = (FCurveKeyframe*) &(buffers[currentIndex + 0].Float);
		currentIndex += 7;

		if (currentIndex == size)
		{
			currentIndex = 0;
			size = 0;
		}

		return ret;
	}
}