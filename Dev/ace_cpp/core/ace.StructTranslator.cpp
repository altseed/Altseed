
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

	void StructTranslator::EnqueueVector3DF(float x, float y, float z)
	{
		buffers.resize(buffers.size() + 3);
		buffers[buffers.size() - 3].Float = x;
		buffers[buffers.size() - 2].Float = y;
		buffers[buffers.size() - 1].Float = z;
	}

	void StructTranslator::EnqueueVector4DF(float x, float y, float z, float w)
	{
		buffers.resize(buffers.size() + 4);
		buffers[buffers.size() - 4].Float = x;
		buffers[buffers.size() - 3].Float = y;
		buffers[buffers.size() - 2].Float = z;
		buffers[buffers.size() - 1].Float = w;
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

	void StructTranslator::EnqueueVector3DF(const Vector3DF& v)
	{
		buffers.resize(buffers.size() + 3);
		buffers[buffers.size() - 3].Float = v.X;
		buffers[buffers.size() - 2].Float = v.Y;
		buffers[buffers.size() - 1].Float = v.Z;
	}

	void StructTranslator::EnqueueVector4DF(const Vector4DF& v)
	{
		buffers.resize(buffers.size() + 4);
		buffers[buffers.size() - 4].Float = v.X;
		buffers[buffers.size() - 3].Float = v.Y;
		buffers[buffers.size() - 2].Float = v.Z;
		buffers[buffers.size() - 1].Float = v.W;
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

	Vector3DF StructTranslator::DequeueVector3DF()
	{
		auto ret0 = buffers[currentIndex + 0].Float;
		auto ret1 = buffers[currentIndex + 1].Float;
		auto ret2 = buffers[currentIndex + 2].Float;

		currentIndex += 3;

		if (currentIndex == buffers.size())
		{
			currentIndex = 0;
			buffers.clear();
		}

		return Vector3DF(ret0, ret1, ret2);
	}

	Vector4DF StructTranslator::DequeueVector4DF()
	{
		auto ret0 = buffers[currentIndex + 0].Float;
		auto ret1 = buffers[currentIndex + 1].Float;
		auto ret2 = buffers[currentIndex + 2].Float;
		auto ret3 = buffers[currentIndex + 3].Float;

		currentIndex += 4;

		if (currentIndex == buffers.size())
		{
			currentIndex = 0;
			buffers.clear();
		}

		return Vector4DF(ret0, ret1, ret2, ret3);
	}
}