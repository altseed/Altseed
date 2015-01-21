#pragma once

#include "../ace.Core.Base.h"
#include "ace.PathIterator.h"
#include <memory>

namespace ace
{
	class PathIterator_Imp : public PathIterator
	{
	private:
		astring::size_type m_pos;
		astring m_element;
		const Path_Imp& m_ppath;

		void Increment(PathIterator_Imp& iterator);
		void Decrement(PathIterator_Imp& iterator);

	public:
		PathIterator_Imp(const Path_Imp& path, astring::size_type pos, const astring& element);
		virtual ~PathIterator_Imp(){};
		virtual int Size() { return m_element.size(); };
		virtual PathIterator_Imp& operator++() override;
		virtual bool operator==(const PathIterator_Imp& itertor);
		virtual bool operator!=(const PathIterator_Imp& iterator);
		virtual bool operator==(const PathIterator& itertor) override { return *this == *dynamic_cast<const PathIterator_Imp*>(&itertor); }
		virtual bool operator!=(const PathIterator& iterator) override { return *this != *dynamic_cast<const PathIterator_Imp*>(&iterator); };
		virtual astring operator*() override;
	};
}