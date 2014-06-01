#include "ace.PathIterator_Imp.h"

namespace ace
{
	void PathIterator_Imp::Increment(PathIterator_Imp& iterator)
	{
	}

	void PathIterator_Imp::Decrement(PathIterator_Imp& iterator)
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
		if (this->m_ppath == iterator.m_ppath &&
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