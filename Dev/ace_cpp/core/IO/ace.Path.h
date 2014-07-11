#pragma once

#include "../ace.Core.Base.h"
#include "ace.PathIterator.h"

namespace ace
{
	class Path
	{
	public:
		virtual ~Path() { };
		virtual astring ToAstring() const = 0;
		virtual std::iterator < std::forward_iterator_tag, Path > Begin() const = 0;
		virtual std::iterator < std::forward_iterator_tag, Path > End() const = 0;
	};
}
