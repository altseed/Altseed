#include "ace.PathIterator_Imp.h"
#include "ace.Path_Imp.h"

namespace ace
{
	void PathIterator_Imp::Increment(PathIterator_Imp& iterator)
	{
		astring::size_type count;
		iterator.m_pos += m_element.size() + 1;

		if (iterator.m_pos < iterator.m_ppath.m_path.size())
		{
			iterator.m_ppath.NextElement(count, iterator.m_pos);
			iterator.m_element = iterator.m_ppath.m_path.substr(iterator.m_pos, count);
		}
		else
		{
			auto& end = iterator.m_ppath.ImpEnd();
			iterator.m_pos = end.m_pos;
			iterator.m_element = end.m_element;
		}
	}

	void PathIterator_Imp::Decrement(PathIterator_Imp& iterator)
	{
	}

	PathIterator_Imp::PathIterator_Imp(const Path_Imp& path, astring::size_type pos, astring& element) :
		m_ppath(path),
		m_pos(pos),
		m_element(element)
	{
	}

	PathIterator_Imp& PathIterator_Imp::operator++()
	{
		Increment(*this);

		return *this;
	}
	PathIterator_Imp PathIterator_Imp::operator++(int)
	{
		PathIterator_Imp tmp(*this);

		Increment(*this);

		return tmp;
	}

	bool PathIterator_Imp::operator==(const PathIterator_Imp& iterator)
	{
		if (&this->m_ppath == &iterator.m_ppath &&
			this->m_element == iterator.m_element &&
			this->m_pos == iterator.m_pos)
		{
			return true;
		}
		else
			return false;
	}

	bool PathIterator_Imp::operator!=(const PathIterator_Imp& iterator)
	{
		return !(*this == iterator);
	}

	astring PathIterator_Imp::operator*()
	{
		return m_element;
	}
}