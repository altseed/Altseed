#pragma once

#include "../ace.Core.Base.h"
#include "../Core/ace.Core.Base.h"
#include "ace.PathIterator.h"

namespace ace
{
	class Path
	{
	public:
		virtual ~Path() { };
		virtual astring ToAstring() const = 0;
		virtual PathIterator begin() const = 0;
		virtual PathIterator end() const = 0;
	};
}