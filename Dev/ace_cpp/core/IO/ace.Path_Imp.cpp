#include "ace.Path_Imp.h"
#include <algorithm>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
//#include <codecvt>
#endif

//#include <iostream>

namespace ace
{
	const achar Path_Imp::m_dotCString[2] = { Path_Imp::m_dot, achar('\0') };

	Path_Imp::Path_Imp()
	{
	}

	Path_Imp::Path_Imp(const astring& path) :
		m_path(path)
	{
		Normalize();
	}

	void Path_Imp::Normalize()
	{
		if (!IsAbsolutePath(m_path))
		{
#ifndef _WIN32
			// realpath で正規化
			std::string u8 = ToUtf8String(m_path.c_str());
			//std::cerr<<"dbg Normalize in:"<<u8<<std::endl;
			std::vector<int8_t> res(4096); //
			realpath(u8.c_str(), reinterpret_cast<char*>(res.data()));
			//std::cerr<<"dbg Normalize res:"<<(char*)res.data()<<std::endl;
			m_path = ToAString(reinterpret_cast<char*>(res.data()));
#else
			achar buffer[MAX_PATH];
			GetModuleFileNameW(nullptr, buffer, MAX_PATH);

			m_path.insert(0, L"/../");
			m_path.insert(0, buffer);
#endif
		}
		if (IsSeparator(m_path[m_path.size() - 1]))
			m_path.append(m_dotCString);

		auto begin_(begin());
		auto end_(end());

		Path_Imp tmp;
		for (auto itr(begin_); itr != end_; ++itr)
		{
			if (itr.Size() == 1	&&
				(*itr)[0] == m_dot &&
				itr != begin_ &&
				itr != end_)
			{
				continue;
			}

			if (!tmp.Empty() &&
				itr.Size() == 2 &&
				(*itr)[0] == m_dot && (*itr)[1] == m_dot)
			{
				tmp.RemoveFileName();
				RemoveLastSeparator();

				continue;
			}

			tmp /= *itr;
		}
		m_path = tmp.m_path;
	}
	void Path_Imp::FirstElement(astring::size_type& start, astring::size_type& count) const
	{
		//std::cerr<<"dbg FirstElement in:"<<ToUtf8String(this->m_path.c_str())<<std::endl;
		assert(IsAbsolutePath(this->m_path));
		// windows
		if (2 <= m_path.size() && m_path[1] == m_colon)
		{
			start = 0;
			count = 2;

			return;
		}
		// linux
		if (1 < m_path.size() && IsSeparator(m_path[0]))
		{
			for (start = 1, count = 0;
				(start+count) < m_path.size() - 1 && !IsSeparator(m_path[start + count]);
				++count);

			return;
		}
	}
	void Path_Imp::NextElement(astring::size_type& count, const astring::size_type& start) const
	{
		astring::size_type s(IsSeparator(m_path[start]) ? start + 1 : start);
		
		for (count = 0;
			(s + count) < m_path.size() && !IsSeparator(m_path[s + count]);
			++count);
	}

	astring Path_Imp::FileName() const
	{
		const auto size = m_path.size();
		astring::size_type start(size-1);

		for (; !IsSeparator(m_path[start]); --start);

		return m_path.substr(start + 1, size - (start + 1));
	}

	void Path_Imp::RemoveFileName()
	{
		const astring fileName(FileName());

		m_path.erase(m_path.size() - fileName.size(), fileName.size());
		RemoveLastSeparator();
	}

	void Path_Imp::RemoveLastSeparator()
	{
		const auto size = m_path.size();

		if (IsSeparator(m_path[size - 1]))
			m_path.erase(size - 1);
	}

	PathIterator Path_Imp::begin() const
	{
		astring::size_type size;
		astring::size_type pos;
		FirstElement(pos, size);

		return PathIterator(*this, pos, m_path.substr(pos, size));
	}

	PathIterator Path_Imp::end() const
	{
		return PathIterator(*this, m_path.size(), astring());
	}

	Path_Imp& Path_Imp::operator/=(const astring& leaf)
	{
		if (leaf.empty())
			return *this;

#ifdef _WIN32
		if (!m_path.empty())
			m_path += m_separator;
#else
		m_path += m_separator;
#endif
		m_path += leaf;

		return *this;
	}

	bool Path_Imp::operator==(const Path_Imp& path) const
	{
		return m_path == path.m_path;
	}

	bool Path_Imp::IsAbsolutePath(const astring& path)
	{
		// win
		if (3 <= path.size())
		{
			if (path[1] == m_colon && IsSeparator(path[2]))
				return true;
		}
		// linux
		if (1 < path.size())
		{
			if (path[0] == m_separator)
				return true;
		}
		return false;
	}
}