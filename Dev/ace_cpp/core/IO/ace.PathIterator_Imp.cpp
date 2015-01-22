#include "ace.PathIterator_Imp.h"
#include "ace.Path_Imp.h"

namespace ace
{
	void PathIterator::Increment(PathIterator& iterator)
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
			const auto& end_ = iterator.m_ppath.end();
			iterator.m_pos = end_.m_pos;
			iterator.m_element = end_.m_element;
		}
	}

	void PathIterator::Decrement(PathIterator& iterator)
	{
	}

	PathIterator::PathIterator(const Path_Imp& path, astring::size_type pos, const astring& element) :
		m_ppath(path),
		m_pos(pos),
		m_element(element)
	{
	}

	int PathIterator::Size() const
	{
		return m_element.size();
	}

	PathIterator& PathIterator::operator++()
	{
		Increment(*this);

		return *this;
	}

	bool PathIterator::operator==(const PathIterator& iterator) const
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

	bool PathIterator::operator!=(const PathIterator& iterator) const
	{
		return !(*this == iterator);
	}

	astring PathIterator::operator*() const
	{
		return m_element;
	}
}
