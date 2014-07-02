#pragma once

#include "../ace.Core.Base.h"
#include "ace.Path.h"
#include "ace.PathIterator_Imp.h"

namespace ace
{
	class Path_Imp : public Path
	{
		friend class PathIterator_Imp;

	protected:
		astring m_path;
		static const achar m_windowsSeparator = achar('\\');
		static const achar m_separator = achar('/');
		static const achar m_colon = achar(':');
		static const achar m_dot = achar('.');
		static const achar m_dotCString[2];

		void FirstElement(astring::size_type& start, astring::size_type& count) const;
		void NextElement(astring::size_type& count, const astring::size_type& start) const;
		astring FileName() const;
		void RemoveFileName();
		void RemoveLastSeparator();
		bool Empty() const { return m_path.empty(); }

	public:
		Path_Imp();
		Path_Imp(const astring& path);
		virtual ~Path_Imp() { };

		void Normalize();
		virtual astring ToAstring() const {	return m_path; };
		virtual PathIterator_Imp ImpBegin() const;
		virtual std::iterator < std::forward_iterator_tag, Path > Begin() const { return ImpBegin(); }
		virtual PathIterator_Imp ImpEnd() const;
		virtual std::iterator < std::forward_iterator_tag, Path > End() const { return ImpEnd(); }

		virtual Path_Imp& operator/=(const astring& leaf);

		static bool IsSeparator(astring::value_type c) { return c == m_windowsSeparator || c == m_separator; };
		static bool IsAbsolutePath(const astring& path);
	};
}