#pragma once

#include "../ace.Core.Base.h"
#include "ace.PathIterator.h"
#include <memory>

namespace ace
{
	class PathIterator_Imp : public PathIterator
	{
	private:
		friend class Path_Imp;

		astring::size_type m_pos;
		astring m_element;
		std::shared_ptr<Path_Imp> m_ppath;

		void Increment(PathIterator_Imp& iterator);
		void Decrement(PathIterator_Imp& iterator);

	public:
		virtual ~PathIterator_Imp(){};
		virtual PathIterator_Imp& operator++();
		virtual PathIterator_Imp operator++(int);
		virtual bool operator==(const PathIterator_Imp& itertor);
		virtual bool operator!=(const PathIterator_Imp& iterator);
		virtual astring operator*();
	};
}