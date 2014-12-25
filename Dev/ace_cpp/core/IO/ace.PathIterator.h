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

		virtual PathIterator& operator++() { return PathIterator(); }
		virtual bool operator==(const PathIterator& itertor) { return false; }
		virtual bool operator!=(const PathIterator& iterator) { return !(*this == iterator); };
		virtual astring operator*() { return ToAString(""); }
	};
}