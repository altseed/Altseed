#pragma once

#include "../ace.Core.Base.h"
#include "ace.Path.h"
#include "ace.PathIterator.h"

namespace ace
{
	class Path_Imp : public Path
	{
	protected:
		astring m_path;
		static const astring m_windowsSeparator;
		static const astring m_separator;
		static const astring m_colon;
		static const astring m_dot;
		static const astring m_wdot;

		void Normalize();
		void FirstElement(astring::size_type& start, astring::size_type& count);
		void NextElement(astring::size_type& count, const astring::size_type& start);
		bool IsSeparator(astring::value_type c);

	public:
		Path_Imp();
		Path_Imp(const astring& path);
		virtual ~Path_Imp() { };
		virtual astring ToAstring() const {	return m_path; };
		virtual PathIterator begin() const;
		virtual PathIterator end() const;
	};
}