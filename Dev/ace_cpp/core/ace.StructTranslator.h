
#pragma once

#include <stdint.h>
#include <queue>

#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Vector4DF.h>

#include <Math/ace.Vector2DI.h>

#include <Graphics/ace.Color.h>

#include <Math/ace.RectI.h>
#include <Math/ace.RectF.h>

#include <Math/ace.Matrix44.h>

#include <Graphics/ace.Graphics.Common.h>

namespace ace
{
	class StructTranslator
	{
	private:
		union Buffer
		{
			int32_t	Int;
			float	Float;
		};
		
		static Buffer				buffers[256];
		static int32_t				size;
		static int32_t				currentIndex;
	public:

		static void EnqueueVector2DF(float x, float y);
		static void EnqueueVector3DF(float x, float y, float z);
		static void EnqueueVector4DF(float x, float y, float z, float w);

		static void EnqueueVector2DI(int32_t x, int32_t y);

		static void EnqueueColor(int32_t r, int32_t g, int32_t b, int32_t a);

		static void EnqueueRectF(float x, float y, float w, float h);
		static void EnqueueRectI(int32_t x, int32_t y, int32_t w, int32_t h);

		static void EnqueueMatrix44(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

		static void EnqueueFCurveKeyframe(
			float kv_x, float kv_y,
			float lh_x, float lh_y,
			float rh_x, float rh_y,
			int32_t i);

		static float DequeueFloat();
		static int32_t DequeueInt();

#if !SWIG
		static void EnqueueVector2DF(const Vector2DF& v);
		static void EnqueueVector3DF(const Vector3DF& v);
		static void EnqueueVector4DF(const Vector4DF& v);

		static void EnqueueVector2DI(const Vector2DI& v);

		static void EnqueueColor(const Color& v);

		static void EnqueueRectF(const RectF& v);
		static void EnqueueRectI(const RectI& v);

		static void EnqueueMatrix44(const Matrix44& v);

		static void EnqueueFCurveKeyframe(const FCurveKeyframe& v);

		static Vector2DF DequeueVector2DF();
		static Vector2DF* Dequeue_Vector2DF();

		static Vector3DF DequeueVector3DF();
		static Vector3DF* Dequeue_Vector3DF();

		static Vector4DF DequeueVector4DF();
		static Vector4DF* Dequeue_Vector4DF();

		static Vector2DI DequeueVector2DI();
		static Vector2DI* Dequeue_Vector2DI();

		static Color DequeueColor();
		static Color* Dequeue_Color();

		static RectF DequeueRectF();
		static RectF* Dequeue_RectF();

		static RectI DequeueRectI();
		static RectI* Dequeue_RectI();

		static Matrix44 DequeueMatrix44();
		static Matrix44* Dequeue_Matrix44();

		static FCurveKeyframe DequeueFCurveKeyframe();
		static FCurveKeyframe* Dequeue_FCurveKeyframe();
#endif
	};
}