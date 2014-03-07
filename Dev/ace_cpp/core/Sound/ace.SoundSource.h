
#pragma once

#include "../ace.Core.Base.h"

namespace ace
{
	class SoundSource
		: public IReference
	{
	protected:
		SoundSource() {}
		~SoundSource() {}

	public:

		virtual void SetLoopPoint(float loopStart, float loopEnd) = 0;

		virtual float GetLength() = 0;
	};
}