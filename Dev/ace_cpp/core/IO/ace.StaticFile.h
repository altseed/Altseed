#pragma once

#include "../ace.ReferenceObject.h"
#include "../ace.Core.Base.h"
#include "ace.File.h"

namespace ace
{
	class StaticFile : public IReference
	{
	private:
	public:
		virtual ~StaticFile() { };
		virtual const std::vector<uint8_t>& ReadAllBytes() const = 0;
	};
}