#pragma once

#include "../ace.ReferenceObject.h"
#include "../ace.Core.Base.h"

namespace ace
{
	class StreamFile : public IReference
	{
	private:
	public:
		virtual ~StreamFile() { };
		virtual void ReadAllByte(std::vector<uint8_t>& buffer) = 0;
	};
}