
#pragma once

#include <stdint.h>
#include <queue>

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
		
		static std::vector<Buffer>	buffers;
		static int32_t				currentIndex;
	public:

		static void EnqueueVector2DF(float x, float y);
		static void EnqueueVector3DF(float x, float y, float z);
		static void EnqueueVector4DF(float x, float y, float z, float w);

		static float DequeueFloat();

#if !SWIG
		static void EnqueueVector2DF(const Vector2DF& v);
		static void EnqueueVector3DF(const Vector3DF& v);
		static void EnqueueVector4DF(const Vector4DF& v);

		static Vector2DF DequeueVector2DF();
		static Vector3DF DequeueVector3DF();
		static Vector4DF DequeueVector4DF();
#endif
	};
}