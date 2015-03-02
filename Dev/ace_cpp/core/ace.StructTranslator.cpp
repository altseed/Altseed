
#include "ace.StructTranslator.h"

namespace ace
{
	std::vector<StructTranslator::Buffer> StructTranslator::buffers = std::vector<StructTranslator::Buffer>();
	int32_t StructTranslator::currentIndex = 0;

	void StructTranslator::EnqueueVector2DF(float x, float y)
	{
		buffers.resize(buffers.size() + 2);
		buffers[buffers.size() - 2].Float = x;
		buffers[buffers.size() - 1].Float = y;
	}

	float StructTranslator::DequeueFloat()
	{
		auto ret = buffers[currentIndex].Float;
		currentIndex++;

		if (currentIndex == buffers.size())
		{
			currentIndex = 0;
			buffers.clear();
		}

		return ret;
	}

	void StructTranslator::EnqueueVector2DF(const Vector2DF& v)
	{
		buffers.resize(buffers.size() + 2);
		buffers[buffers.size() - 2].Float = v.X;
		buffers[buffers.size() - 1].Float = v.Y;
	}

	Vector2DF StructTranslator::DequeueVector2DF()
	{
		auto ret0 = buffers[currentIndex+0].Float;
		auto ret1 = buffers[currentIndex+1].Float;

		currentIndex+=2;

		if (currentIndex == buffers.size())
		{
			currentIndex = 0;
			buffers.clear();
		}

		return Vector2DF(ret0, ret1);
	}
}