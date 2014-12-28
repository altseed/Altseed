#pragma once

#include "../ace.Core.Base.h"
#include "ace.File.h"
#include "ace.Path.h"

namespace ace
{
	class PathIterator
	{
	public:
		virtual ~PathIterator(){ };

		virtual PathIterator& operator++() = 0;
		virtual bool operator==(const PathIterator& itertor) = 0;
		virtual bool operator!=(const PathIterator& iterator) = 0;
		virtual astring operator*() = 0;
	};
}