#pragma once

#include "../ace.Core.Base.h"
#include "ace.File.h"
#include "ace.Path.h"

namespace ace
{
	class PathIterator
	{
	private:
		astring::size_type m_pos;
		astring m_element;
		const Path_Imp& m_ppath;

		void Increment(PathIterator& iterator);
		void Decrement(PathIterator& iterator);

	public:
		PathIterator(const Path_Imp& path, astring::size_type pos, const astring& element);
		virtual ~PathIterator(){}
		virtual int Size() const;
		virtual PathIterator& operator++();
		virtual bool operator==(const PathIterator& itertor) const;
		virtual bool operator!=(const PathIterator& iterator) const;
		virtual astring operator*() const;
	};
}
