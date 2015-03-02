
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
		static float DequeueFloat();

#if !SWIG
		static void EnqueueVector2DF(const Vector2DF& v);
		static Vector2DF DequeueVector2DF();
#endif
	};
}