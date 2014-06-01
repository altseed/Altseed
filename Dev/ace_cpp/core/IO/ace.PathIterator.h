#pragma once

#include "../ace.Core.Base.h"
#include "ace.File.h"
#include "ace.Path.h"

namespace ace
{
	class PathIterator : public std::iterator < std::forward_iterator_tag, Path >
	{
	public:
		virtual ~PathIterator(){ };
	};
}