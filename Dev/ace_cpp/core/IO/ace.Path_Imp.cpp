#include "ace.Path_Imp.h"

namespace ace
{
	Path_Imp::Path_Imp()
	{
	}

	Path_Imp::Path_Imp(const astring& path)
	{
		m_path = path;
		Normalize();
	}

	void Path_Imp::Normalize()
	{
	}
	void Path_Imp::FirstElement(astring::size_type& start, astring::size_type& count)
	{
	}
	void Path_Imp::NextElement(astring::size_type& count, const astring::size_type& start)
	{
	}
	bool Path_Imp::IsSeparator(astring::value_type c)
	{
#ifdef _WIN32
#else
#endif
		return true;
	}

	PathIterator Path_Imp::begin() const
	{
		assert(false);
		return PathIterator();
	}

	PathIterator Path_Imp::end() const
	{
		assert(false);
		return PathIterator();
	}
}